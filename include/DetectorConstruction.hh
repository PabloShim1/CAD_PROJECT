#ifndef TyOneDetectorConstruction_h
#define TyOneDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4Material.hh"
#include "G4ThreeVector.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4VoxelLimits.hh"
#include "G4AffineTransform.hh"

namespace tyone
{
  class DetectorConstruction : public G4VUserDetectorConstruction
  {
  public:
    DetectorConstruction();
    ~DetectorConstruction() override;
    G4VPhysicalVolume *Construct() override;

    // Геттеры для PrimaryGeneratorAction
    inline G4ThreeVector GetSourceTranslate() const noexcept { return source_translate; };

    // Метод для расчета реальных габаритов (Bounding Box)
    G4ThreeVector GetGloveFullSizes() const {
        if (!glove_logic) return G4ThreeVector(0,0,0);
        G4double xmin, xmax, ymin, ymax, zmin, zmax;
        glove_logic->GetSolid()->CalculateExtent(kXAxis, G4VoxelLimits(), G4AffineTransform(), xmin, xmax);
        glove_logic->GetSolid()->CalculateExtent(kYAxis, G4VoxelLimits(), G4AffineTransform(), ymin, ymax);
        glove_logic->GetSolid()->CalculateExtent(kZAxis, G4VoxelLimits(), G4AffineTransform(), zmin, zmax);
        return G4ThreeVector(xmax - xmin, ymax - ymin, zmax - zmin);
    }

    // Метод для расчета геометрического центра модели
    G4ThreeVector GetGloveCenter() const {
        if (!glove_logic) return G4ThreeVector(0,0,0);
        G4double xmin, xmax, ymin, ymax, zmin, zmax;
        glove_logic->GetSolid()->CalculateExtent(kXAxis, G4VoxelLimits(), G4AffineTransform(), xmin, xmax);
        glove_logic->GetSolid()->CalculateExtent(kYAxis, G4VoxelLimits(), G4AffineTransform(), ymin, ymax);
        glove_logic->GetSolid()->CalculateExtent(kZAxis, G4VoxelLimits(), G4AffineTransform(), zmin, zmax);
        return G4ThreeVector((xmin + xmax)/2.0, (ymin + ymax)/2.0, (zmin + zmax)/2.0);
    }

  private:
    void ConstructMaterials();
    void ComputeTranslations();
    void ConstructSolids();
    void ConstructLogicals();
    void ConstructPhysicals();
    void SetUpVisAttributes();

  private:
    G4double margin_x, margin_y, margin_z;
    G4double world_size_x, world_size_y, world_size_z;
    
    G4Material *world_material = nullptr;
    G4Box *world_solid = nullptr;
    G4LogicalVolume *world_logic = nullptr;
    G4VPhysicalVolume *world_phys = nullptr;

    G4Material *glove_material = nullptr;
    G4LogicalVolume *glove_logic = nullptr;
    
    G4ThreeVector source_translate;
    G4double source_surface_distance; // Вот она!
  };
}
#endif