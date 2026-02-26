#include "SteppingAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

namespace tyone
{

  SteppingAction::SteppingAction()
  {
    // get general detector construction
    // static_cast<const DetecorConstruction*>(general detector construction);
    // sensitive_volume = detector->GetSensitiveVolume();
  }

  SteppingAction::~SteppingAction()
  {
  }

  void SteppingAction::UserSteppingAction(const G4Step *step)
  {
    // if (volume != sensitive_volume) return;
    // Fill hist 3D.
  }
}
