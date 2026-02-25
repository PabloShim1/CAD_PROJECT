#ifndef TyOneDetectorConstruction_h
#define TyOneDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4Material.hh"
#include "G4ThreeVector.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"

namespace tyone
{
  class DetectorConstruction : public G4VUserDetectorConstruction
  {
  public:
    DetectorConstruction();
    ~DetectorConstruction() override;
    G4VPhysicalVolume *Construct() override;

    // Геттеры
    inline const G4Box *GetWorldSolid() const noexcept { return world_solid; };
    inline const G4LogicalVolume *GetWorldLogic() const noexcept { return world_logic; };
    inline const G4VPhysicalVolume *GetWorldPhys() const noexcept { return world_phys; };

  private:
    void ConstructMaterials();
    void ComputeTranslations();
    void ConstructSolids();
    void ConstructLogicals();
    void ConstructPhysicals();
    void SetUpVisAttributes();

  private:
    // Параметры мира
    G4double margin_x, margin_y, margin_z;
    G4double world_size_x, world_size_y, world_size_z;
    
    // Мир
    G4Material *world_material = nullptr;
    G4Box *world_solid = nullptr;
    G4LogicalVolume *world_logic = nullptr;
    G4VPhysicalVolume *world_phys = nullptr;

    // Модель (Перчатка)
    G4Material *glove_material = nullptr;
    G4LogicalVolume *glove_logic = nullptr;
    
    // Вспомогательные переменные
    G4ThreeVector source_translate;
    G4double source_surface_distance;
  };
}
#endif