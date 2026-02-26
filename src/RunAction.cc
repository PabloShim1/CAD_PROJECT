#include "RunAction.hh"
#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"

namespace tyone
{
  RunAction::RunAction()
  {
    auto analysis = G4AnalysisManager::Instance();
    analysis->SetDefaultFileType("root");
    analysis->SetFirstH1Id(0);
    analysis->SetFirstH2Id(0);
    analysis->SetFirstH3Id(0);
    analysis->SetFirstNtupleId(0);
    analysis->SetFirstNtupleColumnId(0);
  }

  RunAction::~RunAction()
  {
  }

  void RunAction::BeginOfRunAction(const G4Run *)
  {
    // optional: clear all the old data
    // open file with specific name
    // create Hist3D
  }

  void RunAction::EndOfRunAction(const G4Run *run)
  {
    auto analysis = G4AnalysisManager::Instance();
    analysis->Write();
    analysis->CloseFile();
  }
}
