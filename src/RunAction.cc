#include "RunAction.hh"
#include "GlobalConfig.hh"
#include "DetectorConstruction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include <cmath>

namespace tyone
{
  RunAction::RunAction() : G4UserRunAction()
  {
    // Создаем экземпляр менеджера анализа
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetDefaultFileType("root");
    analysisManager->SetVerboseLevel(1);
  }

  void RunAction::BeginOfRunAction(const G4Run*)
  {
    auto analysisManager = G4AnalysisManager::Instance();

    // Открываем файл, имя которого задано в GlobalConfig.hh
    analysisManager->OpenFile(OUTPUT_FILE);

    // Получаем доступ к детектору, чтобы узнать размеры модели
    auto detConstruction = static_cast<const DetectorConstruction*>(
        G4RunManager::GetRunManager()->GetUserDetectorConstruction());
    
    G4ThreeVector modelSize = detConstruction->GetGloveFullSizes();

    // Рассчитываем полу-границы сетки с учетом отступа PADDING
    G4double halfX = (modelSize.x() + 2 * MESH_PADDING) / 2.0;
    G4double halfY = (modelSize.y() + 2 * MESH_PADDING) / 2.0;
    G4double halfZ = (modelSize.z() + 2 * MESH_PADDING) / 2.0;

    // Рассчитываем количество бинов (вокселей)
    // std::max нужен, чтобы не получить 0 бинов случайно
    G4int nBinsX = std::max(1, (int)std::ceil((2 * halfX) / MESH_STEP));
    G4int nBinsY = std::max(1, (int)std::ceil((2 * halfY) / MESH_STEP));
    G4int nBinsZ = std::max(1, (int)std::ceil((2 * halfZ) / MESH_STEP));

    // Создаем 3D гистограмму для карты энерговыделения
    // ID будет 0. Имя "EdepMap" ты будешь использовать в uproot в Jupyter
    analysisManager->CreateH3("EdepMap", "Energy Deposit Distribution",
                              nBinsX, -halfX, halfX,
                              nBinsY, -halfY, halfY,
                              nBinsZ, -halfZ, halfZ);
    
    G4cout << "-> RunAction: 3D Histogram created with bins: " 
           << nBinsX << "x" << nBinsY << "x" << nBinsZ << G4endl;
  }

  void RunAction::EndOfRunAction(const G4Run*)
  {
    auto analysisManager = G4AnalysisManager::Instance();

    // Сохраняем данные и закрываем файл
    analysisManager->Write();
    analysisManager->CloseFile();

    G4cout << "-> RunAction: Data saved to " << OUTPUT_FILE << ".root" << G4endl;
  }
}