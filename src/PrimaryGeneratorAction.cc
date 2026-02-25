#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

namespace tyone
{
  PrimaryGeneratorAction::PrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction()
  {
    fParticleGun = new G4ParticleGun(1);
    auto particle = G4ParticleTable::GetParticleTable()->FindParticle("gamma");
    fParticleGun->SetParticleDefinition(particle);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(1.0, 0.0, 0.0));
    fParticleGun->SetParticleEnergy(662.*keV);
  }

  PrimaryGeneratorAction::~PrimaryGeneratorAction() { delete fParticleGun; }

  void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
  {
    auto det = static_cast<const DetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());

    G4ThreeVector cadSize = det->GetGloveFullSizes();
    G4ThreeVector cadCenter = det->GetGloveCenter();

    // Центрируем пучок относительно реального центра модели
    G4double randY = cadCenter.y() + (G4UniformRand() - 0.5) * cadSize.y();
    G4double randZ = cadCenter.z() + (G4UniformRand() - 0.5) * cadSize.z();
    G4double startX = det->GetSourceTranslate().x();

    fParticleGun->SetParticlePosition(G4ThreeVector(startX, randY, randZ));
    fParticleGun->GeneratePrimaryVertex(anEvent);
  }
}