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
        this->margin_x = 100 * cm;
        this->margin_y = 100 * cm;
        this->margin_z = 100 * cm; 
        this->source_surface_distance = 0.0; // Добавили .0 для double
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
        // Задаем точку вылета (например, 40 см слева от центра)
        this->source_translate = G4ThreeVector(-40.0 * cm, 0, 0);
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
        this->world_logic = new G4LogicalVolume(this->world_solid, this->world_material, "World_Logic");

        auto mesh = CADMesh::TessellatedMesh::FromOBJ("glove1.obj");
        mesh->SetScale(10.0); 
        mesh->SetOffset(G4ThreeVector(0, 0, 0)); 

        G4VSolid* glove_solid = mesh->GetSolid();
        this->glove_logic = new G4LogicalVolume(glove_solid, this->glove_material, "Glove_Logic");

        G4VisAttributes* glove_vis = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0, 1.0));
        glove_vis->SetForceSolid(true); 
        this->glove_logic->SetVisAttributes(glove_vis);
    }

    void DetectorConstruction::ConstructPhysicals()
    {
        this->world_phys = new G4PVPlacement(nullptr, G4ThreeVector(), this->world_logic, 
                                             "World_Phys", nullptr, false, 0, true);

        new G4PVPlacement(nullptr, G4ThreeVector(0, 0, 0), this->glove_logic, 
                          "Glove_Phys", this->world_logic, false, 0, true);
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