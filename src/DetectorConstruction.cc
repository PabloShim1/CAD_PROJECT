#include "DetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Color.hh"

// Подключаем CADMesh
#include "CADMesh.hh"

namespace tyone
{
    DetectorConstruction::DetectorConstruction()
    {
        this->margin_x = 100 * cm;
        this->margin_y = 100 * cm;
        this->margin_z = 100 * cm; 
    }

    DetectorConstruction::~DetectorConstruction()
    {
    }

    void DetectorConstruction::ConstructMaterials()
    {
        G4NistManager *nist = G4NistManager::Instance();
        this->world_material = nist->FindOrBuildMaterial("G4_AIR");
        nist->FindOrBuildMaterial("G4_POLYETHYLENE");
        nist->FindOrBuildMaterial("G4_Al");
    }

    void DetectorConstruction::ComputeTranslations()
    {
        this->world_size_x = this->margin_x;
        this->world_size_y = this->margin_y;
        this->world_size_z = this->margin_z;
    }

    void DetectorConstruction::ConstructSolids()
    {
        this->world_solid = new G4Box("World", 
                                      this->world_size_x / 2.0, 
                                      this->world_size_y / 2.0, 
                                      this->world_size_z / 2.0);
    }

    void DetectorConstruction::ConstructLogicals()
    {
        G4NistManager *nist = G4NistManager::Instance();
        
        // 1. Создаем мир
        this->world_logic = new G4LogicalVolume(this->world_solid, this->world_material, "World");

        // 2. ЗАГРУЗКА ПЕРЧАТКИ (КРАСНАЯ, СЛЕВА)
        auto mesh = CADMesh::TessellatedMesh::FromOBJ("glove1.obj");
        
        // Увеличиваем масштаб, так как в логе координаты были около 0.5
        mesh->SetScale(10); 

        G4VSolid* glove_solid = mesh->GetSolid();
        G4LogicalVolume* glove_logic = new G4LogicalVolume(
            glove_solid, 
            nist->FindOrBuildMaterial("G4_POLYETHYLENE"), 
            "Glove_Logic"
        );

        // Цвет: Ярко-красный, непрозрачный
        G4VisAttributes* glove_vis = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0, 1.0));
        glove_vis->SetForceSolid(true); 
        glove_logic->SetVisAttributes(glove_vis);

        // Размещаем СЛЕВА (-20 см)
        new G4PVPlacement(nullptr, 
                          G4ThreeVector(0, 0, 0), 
                          glove_logic, 
                          "Glove_Phys", 
                          this->world_logic, 
                          false, 
                          0, 
                          true);

        // 3. ТЕСТОВЫЙ КУБ (СИНИЙ, СПРАВА)
        G4Box* box_solid = new G4Box("TestBox", 5*cm, 5*cm, 5*cm);
        G4LogicalVolume* box_logic = new G4LogicalVolume(box_solid, nist->FindOrBuildMaterial("G4_Al"), "Box_Logic");
        
        G4VisAttributes* box_vis = new G4VisAttributes(G4Colour(0.0, 0.0, 1.0, 0.4));
        box_vis->SetForceSolid(true);
        box_logic->SetVisAttributes(box_vis);

        // Размещаем СПРАВА (+20 см)
        new G4PVPlacement(nullptr, 
                          G4ThreeVector(20*cm, 0, 0), 
                          box_logic, 
                          "Box_Phys", 
                          this->world_logic, 
                          false, 
                          0, 
                          true);
    }

    void DetectorConstruction::ConstructPhysicals()
    {
        this->world_phys = new G4PVPlacement(
            nullptr,
            G4ThreeVector(),
            this->world_logic,
            "World",
            nullptr,
            false,
            0,
            true);
    }

    void DetectorConstruction::SetUpVisAttributes()
    {
        G4VisAttributes *world_vis = new G4VisAttributes();
        world_vis->SetVisibility(false); 
        this->world_logic->SetVisAttributes(world_vis);
    }

    G4VPhysicalVolume *DetectorConstruction::Construct()
    {
        this->ConstructMaterials();
        this->ComputeTranslations();
        this->ConstructSolids();
        this->ConstructLogicals();
        this->ConstructPhysicals();
        this->SetUpVisAttributes();

        return this->world_phys;
    }
}