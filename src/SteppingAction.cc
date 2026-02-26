#include "SteppingAction.hh"
#include "DetectorConstruction.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"

namespace tyone
{
  SteppingAction::SteppingAction() : G4UserSteppingAction() {}
  SteppingAction::~SteppingAction() {}

  void SteppingAction::UserSteppingAction(const G4Step *step)
  {
    auto det = static_cast<const DetectorConstruction*>(
        G4RunManager::GetRunManager()->GetUserDetectorConstruction()
    );

    auto volume = step->GetPreStepPoint()->GetTouchableHandle()->GetVolume()->GetLogicalVolume();

    // ПРОВЕРКА: используем имя, которое есть в DetectorConstruction
    if (volume != det->GetGloveLogic()) return;
    G4double edep = step->GetTotalEnergyDeposit();
    if (edep <= 0.) return;
    // G4cout << "xxxyyyxxx\n";

    G4ThreeVector pos = step->GetPostStepPoint()->GetPosition();
    G4AnalysisManager::Instance()->FillH3(0, pos.x(), pos.y(), pos.z(), edep);
  }
}