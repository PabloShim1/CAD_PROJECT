#ifndef TyOneDetectorConstruction_h
#define TyOneDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4Box;

namespace tyone
{
  class DetectorConstruction : public G4VUserDetectorConstruction
  {
  public:
    DetectorConstruction();
    ~DetectorConstruction() override;

    G4VPhysicalVolume *Construct() override;
    // ВАЖНО: Этот метод должен быть объявлен здесь!
    void ConstructSDandField() override; 

    // Геттеры
    inline G4bool IsRotated() const { return fUseRotation; }
    G4ThreeVector GetGloveFullSizes() const;
    G4ThreeVector GetGloveCenter() const;

  private:
    void ConstructMaterials();
    void ConstructLogicals();
    void ConstructPhysicals();

  private:
    G4double margin_x, margin_y, margin_z;
    G4Material *world_material = nullptr;
    G4LogicalVolume *world_logic = nullptr;
    G4VPhysicalVolume *world_phys = nullptr;

    G4Material *glove_material = nullptr;
    G4LogicalVolume *glove_logic = nullptr;
    
    G4bool fUseRotation;
    G4RotationMatrix* fGloveRotation = nullptr;
    G4Box* world_solid = nullptr;
  };
}
#endif