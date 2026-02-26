#include "DetectorConstruction.hh"
#include "G4NistManager.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "CADMesh.hh"
#include "G4VoxelLimits.hh"
#include "G4AffineTransform.hh"

namespace tyone
{
    DetectorConstruction::DetectorConstruction()
        : G4VUserDetectorConstruction(), fGloveRotation(nullptr)
    {
        this->margin_x = 100 * cm;
        this->margin_y = 100 * cm;
        this->margin_z = 100 * cm; 
        this->fUseRotation = true; // Оставляем true для твоей модели
    }

    DetectorConstruction::~DetectorConstruction() { delete fGloveRotation; }

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
        this->source_translate = G4ThreeVector(0, 0, -40 * cm);
    }

    void DetectorConstruction::ConstructSolids()
    {
        this->world_solid = new G4Box("World_Solid", this->world_size_x/2., this->world_size_y/2., this->world_size_z/2.);
    }

    void DetectorConstruction::ConstructLogicals()
    {
        this->world_logic = new G4LogicalVolume(this->world_solid, this->world_material, "World_Logic");

        auto mesh = CADMesh::TessellatedMesh::FromOBJ("glove1.obj");
        mesh->SetScale(10.0); 
        
        // --- РЕШЕНИЕ ПРОБЛЕМЫ СМЕЩЕНИЯ ---
        // Сначала считаем центр "сырого" меша
        G4VSolid* tempSolid = mesh->GetSolid();
        G4double xmin, xmax, ymin, ymax, zmin, zmax;
        tempSolid->CalculateExtent(kXAxis, G4VoxelLimits(), G4AffineTransform(), xmin, xmax);
        tempSolid->CalculateExtent(kYAxis, G4VoxelLimits(), G4AffineTransform(), ymin, ymax);
        tempSolid->CalculateExtent(kZAxis, G4VoxelLimits(), G4AffineTransform(), zmin, zmax);
        G4ThreeVector center((xmin+xmax)/2., (ymin+ymax)/2., (zmin+zmax)/2.);
        
        // Сдвигаем меш так, чтобы его собственный центр стал точкой (0,0,0)
        mesh->SetOffset(-center); 

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

        // Теперь модель ВСЕГДА центрирована в (0,0,0) своего объема.
        // Размещаем её в нуле Мира. Вращение произойдет вокруг центра модели.
        new G4PVPlacement(fGloveRotation, 
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
        G4VisAttributes *world_vis = new G4VisAttributes(false);
        this->world_logic->SetVisAttributes(world_vis);
    }

    // Эти методы теперь возвращают размеры уже центрированного объекта
    G4ThreeVector DetectorConstruction::GetGloveFullSizes() const {
        if (!glove_logic) return G4ThreeVector(0,0,0);
        G4double xmin, xmax, ymin, ymax, zmin, zmax;
        glove_logic->GetSolid()->CalculateExtent(kXAxis, G4VoxelLimits(), G4AffineTransform(), xmin, xmax);
        glove_logic->GetSolid()->CalculateExtent(kYAxis, G4VoxelLimits(), G4AffineTransform(), ymin, ymax);
        glove_logic->GetSolid()->CalculateExtent(kZAxis, G4VoxelLimits(), G4AffineTransform(), zmin, zmax);
        return G4ThreeVector(xmax - xmin, ymax - ymin, zmax - zmin);
    }

    G4ThreeVector DetectorConstruction::GetGloveCenter() const {
        return G4ThreeVector(0,0,0); // Модель теперь всегда в нуле
    }

    G4VPhysicalVolume *DetectorConstruction::Construct()
    {
        ConstructMaterials(); ComputeTranslations(); ConstructSolids();
        ConstructLogicals(); ConstructPhysicals(); SetUpVisAttributes();
        return this->world_phys;
    }
}