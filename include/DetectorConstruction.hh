#ifndef TyOneDetectorConstruction_h
#define TyOneDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Box.hh"

namespace tyone
{
  class DetectorConstruction : public G4VUserDetectorConstruction
  {
  public:
    DetectorConstruction();
    ~DetectorConstruction() override;
    G4VPhysicalVolume *Construct() override;

    // Геттеры
    inline G4ThreeVector GetSourceTranslate() const noexcept { return source_translate; };
    inline G4bool IsRotated() const { return fUseRotation; } // НОВЫЙ ГЕТТЕР

    G4ThreeVector GetGloveFullSizes() const;
    G4ThreeVector GetGloveCenter() const;
    G4LogicalVolume* GetGloveLogic() const { return glove_logic; }
    // const G4LogicalVolume* GetSensitiveVolume() const { return this->sensitive_volume; };

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
    G4double source_surface_distance;

    G4bool fUseRotation;
    G4RotationMatrix* fGloveRotation;

    // G4LogicalVolume* sensitive_volume;
  };
}
#endif