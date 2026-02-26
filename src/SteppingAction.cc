#include "SteppingAction.hh"
#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4LogicalVolume.hh"

namespace tyone
{
  SteppingAction::SteppingAction() : G4UserSteppingAction() {}

  void SteppingAction::UserSteppingAction(const G4Step* step)
  {
    // 1. Проверяем, было ли выделение энергии на этом шаге
    G4double edep = step->GetTotalEnergyDeposit();
    if (edep <= 0.) return;

    // 2. Получаем логический объем, в котором находится частица
    auto volume = step->GetPreStepPoint()->GetTouchableHandle()
                      ->GetVolume()->GetLogicalVolume();

    // 3. Если мы внутри перчатки (имя должно совпадать с DetectorConstruction)
    if (volume->GetName() == "Glove_Logic") {
        
        // Берем координаты середины шага для точности
        G4ThreeVector pos = (step->GetPreStepPoint()->GetPosition() + 
                             step->GetPostStepPoint()->GetPosition()) * 0.5;

        // Записываем в 3D-гистограмму (индекс 0, который мы создали в RunAction)
        auto analysisManager = G4AnalysisManager::Instance();
        analysisManager->FillH3(0, pos.x(), pos.y(), pos.z(), edep);
    }
  }
}