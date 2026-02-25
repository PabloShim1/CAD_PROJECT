#include "DetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "CADMesh.hh"

namespace tyone
{
    DetectorConstruction::DetectorConstruction()
        : G4VUserDetectorConstruction()
    {
        // Инициализация размеров
        this->margin_x = 100 * cm;
        this->margin_y = 100 * cm;
        this->margin_z = 100 * cm; 
        this->source_surface_distance = 0;
    }

    DetectorConstruction::~DetectorConstruction() {}

    void DetectorConstruction::ConstructMaterials()
    {
        G4NistManager *nist = G4NistManager::Instance();
        this->world_material = nist->FindOrBuildMaterial("G4_AIR");
        this->glove_material = nist->FindOrBuildMaterial("G4_POLYETHYLENE");
    }

    void DetectorConstruction::ComputeTranslations()
    {
        this->world_size_x = this->margin_x;
        this->world_size_y = this->margin_y;
        this->world_size_z = this->margin_z;
    }

    void DetectorConstruction::ConstructSolids()
    {
        this->world_solid = new G4Box("World_Solid", 
                                      this->world_size_x / 2.0, 
                                      this->world_size_y / 2.0, 
                                      this->world_size_z / 2.0);
    }

    void DetectorConstruction::ConstructLogicals()
    {
        // 1. Логика мира
        this->world_logic = new G4LogicalVolume(this->world_solid, this->world_material, "World_Logic");

        // 2. Загрузка и центрирование модели через CADMesh
        auto mesh = CADMesh::TessellatedMesh::FromOBJ("glove1.obj");
        
        mesh->SetScale(10.0); // Твой масштаб
        mesh->SetOffset(G4ThreeVector(0, 0, 0)); // Центрируем меш внутри его солида

        G4VSolid* glove_solid = mesh->GetSolid();
        
        this->glove_logic = new G4LogicalVolume(
            glove_solid, 
            this->glove_material, 
            "Glove_Logic"
        );

        // Визуализация: оставляем красный, раз тебе он нравится, но без лишних объектов
        G4VisAttributes* glove_vis = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0, 1.0));
        glove_vis->SetForceSolid(true); 
        this->glove_logic->SetVisAttributes(glove_vis);
    }

    void DetectorConstruction::ConstructPhysicals()
    {
        // Размещаем Мир
        this->world_phys = new G4PVPlacement(
            nullptr,
            G4ThreeVector(),
            this->world_logic,
            "World_Phys",
            nullptr,
            false,
            0,
            true);

        // Размещаем Перчатку точно в центре (0,0,0) мира
        new G4PVPlacement(nullptr, 
                          G4ThreeVector(0, 0, 0), 
                          this->glove_logic, 
                          "Glove_Phys", 
                          this->world_logic, 
                          false, 
                          0, 
                          true);
    }

    void DetectorConstruction::SetUpVisAttributes()
    {
        G4VisAttributes *world_vis = new G4VisAttributes();
        world_vis->SetVisibility(false); // Делаем мир невидимым, чтобы не мешал
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