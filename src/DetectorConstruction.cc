#include "DetectorConstruction.hh"
#include "GlobalConfig.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "CADMesh.hh"
#include "G4VoxelLimits.hh"
#include "G4AffineTransform.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
// ИНКЛЮЗЫ ДЛЯ ДЕТЕКТОРА (Проверь их наличие!)
#include "G4SDManager.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4VPrimitiveScorer.hh"
#include "G4PSEnergyDeposit.hh"

namespace tyone
{
    DetectorConstruction::DetectorConstruction()
        : G4VUserDetectorConstruction(), fGloveRotation(nullptr)
    {
        this->margin_x = 100 * cm;
        this->margin_y = 100 * cm;
        this->margin_z = 100 * cm; 
        this->fUseRotation = true; 
    }

    DetectorConstruction::~DetectorConstruction() { delete fGloveRotation; }

    void DetectorConstruction::ConstructMaterials()
    {
        G4NistManager *nist = G4NistManager::Instance();
        this->world_material = nist->FindOrBuildMaterial("G4_AIR");
        this->glove_material = nist->FindOrBuildMaterial("G4_POLYETHYLENE");
    }

    void DetectorConstruction::ConstructLogicals()
    {
        this->world_solid = new G4Box("World_Solid", margin_x/2., margin_y/2., margin_z/2.);
        this->world_logic = new G4LogicalVolume(this->world_solid, this->world_material, "World_Logic");

        auto mesh = CADMesh::TessellatedMesh::FromOBJ("glove1.obj");
        mesh->SetScale(10.0); 
        
        G4VSolid* tempSolid = mesh->GetSolid();
        G4double xmin, xmax, ymin, ymax, zmin, zmax;
        tempSolid->CalculateExtent(kXAxis, G4VoxelLimits(), G4AffineTransform(), xmin, xmax);
        tempSolid->CalculateExtent(kYAxis, G4VoxelLimits(), G4AffineTransform(), ymin, ymax);
        tempSolid->CalculateExtent(kZAxis, G4VoxelLimits(), G4AffineTransform(), zmin, zmax);
        mesh->SetOffset(-G4ThreeVector((xmin+xmax)/2., (ymin+ymax)/2., (zmin+zmax)/2.)); 

        this->glove_logic = new G4LogicalVolume(mesh->GetSolid(), this->glove_material, "Glove_Logic");

        G4VisAttributes* glove_vis = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0, 1.0));
        glove_vis->SetForceSolid(true); 
        this->glove_logic->SetVisAttributes(glove_vis);
    }

    void DetectorConstruction::ConstructPhysicals()
    {
        this->world_phys = new G4PVPlacement(nullptr, G4ThreeVector(), this->world_logic, "World_Phys", nullptr, false, 0, true);

        if (this->fUseRotation) {
            if (!fGloveRotation) fGloveRotation = new G4RotationMatrix();
            fGloveRotation->rotateX(90 * deg);
        }

        new G4PVPlacement(fGloveRotation, G4ThreeVector(0, 0, 0), this->glove_logic, "Glove_Phys", this->world_logic, false, 0, true);
    }

    // РЕАЛИЗАЦИЯ МЕТОДА
    void DetectorConstruction::ConstructSDandField()
{
    if (SCORING_METHOD == 1) {
        G4SDManager* sdManager = G4SDManager::GetSDMpointer();
        
        // 1. Создаем детектор
        G4MultiFunctionalDetector* det = new G4MultiFunctionalDetector("GloveDetector");
        sdManager->AddNewDetector(det);

        // 2. Создаем скорер
        G4VPrimitiveScorer* primitive = new G4PSEnergyDeposit("Edep");
        det->RegisterPrimitive(primitive);

        // 3. ПРЯМОЕ НАЗНАЧЕНИЕ (вместо SetSensitiveDetector)
        if (this->glove_logic) {
            this->glove_logic->SetSensitiveDetector(det);
        }
    }
}

    G4ThreeVector DetectorConstruction::GetGloveFullSizes() const {
        if (!glove_logic) return G4ThreeVector(0,0,0);
        G4double xmin, xmax, ymin, ymax, zmin, zmax;
        glove_logic->GetSolid()->CalculateExtent(kXAxis, G4VoxelLimits(), G4AffineTransform(), xmin, xmax);
        glove_logic->GetSolid()->CalculateExtent(kYAxis, G4VoxelLimits(), G4AffineTransform(), ymin, ymax);
        glove_logic->GetSolid()->CalculateExtent(kZAxis, G4VoxelLimits(), G4AffineTransform(), zmin, zmax);
        return G4ThreeVector(xmax - xmin, ymax - ymin, zmax - zmin);
    }

    G4VPhysicalVolume *DetectorConstruction::Construct()
    {
        ConstructMaterials();
        ConstructLogicals();
        ConstructPhysicals();
        return this->world_phys;
    }
}