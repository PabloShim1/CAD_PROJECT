#include "RunAction.hh"
#include "DetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"

namespace tyone
{
  // Конструктор
  RunAction::RunAction() : G4UserRunAction()
  {
    auto analysis = G4AnalysisManager::Instance();
    analysis->SetDefaultFileType("root");

    // Извлекаем размеры для сетки
    auto det = static_cast<const DetectorConstruction*>(
        G4RunManager::GetRunManager()->GetUserDetectorConstruction()
    );
    G4ThreeVector sz = det->GetGloveFullSizes();
    if (sz.mag2() == 0) sz = G4ThreeVector(20*cm, 20*cm, 20*cm);

    // Создаем гистограмму (ID = 0)
    analysis->CreateH3("dose", "Dose Map", 
                       100, -sz.x()/2, sz.x()/2, 
                       100, -sz.y()/2, sz.y()/2, 
                       100, -sz.z()/2, sz.z()/2,
                       "cm", "cm", "cm");
  }

  // Деструктор (ОБЯЗАТЕЛЬНО должен быть реализован)
  RunAction::~RunAction() 
  {}

  void RunAction::BeginOfRunAction(const G4Run*)
  {
    auto analysis = G4AnalysisManager::Instance();
    analysis->OpenFile("glove_data.root");
  }

  void RunAction::EndOfRunAction(const G4Run*)
  {
    auto analysis = G4AnalysisManager::Instance();
    analysis->Write();
    analysis->CloseFile();
  }
}