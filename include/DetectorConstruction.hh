#ifndef TyOneDetectorConstruction_h
#define TyOneDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

#include "G4Material.hh"
#include "G4ThreeVector.hh"
#include "G4RotationMatrix.hh"

#include "G4Box.hh"
#include "G4VSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4VPhysicalVolume.hh"


namespace tyone
{

  class DetectorConstruction : public G4VUserDetectorConstruction
  {
  public:
    DetectorConstruction();
    ~DetectorConstruction() override;

  public:
    G4VPhysicalVolume *Construct() override;

  public:
    inline const G4Box *GetWorldSolid() const noexcept { return world_solid; };
    inline const G4LogicalVolume *GetWorldLogic() const noexcept { return world_logic; };
    inline const G4VPhysicalVolume *GetWorldPhys() const noexcept { return world_phys; };

    inline const G4ThreeVector GetSourceTraslate() const noexcept { return this->source_translate; };
    inline const G4double GetSSD() const noexcept { return source_surface_distance; };

  private:
    void ConstructMaterials();
    void ComputeTranslations();

    void ConstructSolids();
    void ConstructLogicals();
    void ConstructPhysicals();
    void SetUpRegionsAndCuts();
    void SetUpVisAttributes();

  private:
    G4double margin_x, margin_y, margin_z;
    G4double world_size_x, world_size_y, world_size_z;
    G4Material *world_material = nullptr;
    G4Box *world_solid = nullptr;
    G4LogicalVolume *world_logic = nullptr;
    G4VPhysicalVolume *world_phys = nullptr;

    //
    G4ThreeVector source_translate;
    G4double source_surface_distance;
  };

}

#endif
