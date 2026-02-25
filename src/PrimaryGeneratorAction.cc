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
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.0, 0.0, 1.0)); // Светим вверх
    fParticleGun->SetParticleEnergy(662.*keV);
  }

  PrimaryGeneratorAction::~PrimaryGeneratorAction() { delete fParticleGun; }

  void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
  {
    auto det = static_cast<const DetectorConstruction*>(G4RunManager::GetRunManager()->GetUserDetectorConstruction());

    G4ThreeVector rawSize = det->GetGloveFullSizes();
    G4double bWidthX, bWidthY;

    // Адаптация размеров пучка под поворот
    if (det->IsRotated()) {
        bWidthX = rawSize.x();
        bWidthY = rawSize.z(); // Бывшая высота теперь ширина
    } else {
        bWidthX = rawSize.x();
        bWidthY = rawSize.y();
    }

    // Так как модель в (0,0,0), рандомизируем симметрично вокруг нуля
    G4double randX = (G4UniformRand() - 0.5) * bWidthX;
    G4double randY = (G4UniformRand() - 0.5) * bWidthY;
    G4double startZ = -40.0 * cm; // Источник ниже центра модели

    fParticleGun->SetParticlePosition(G4ThreeVector(randX, randY, startZ));
    fParticleGun->GeneratePrimaryVertex(anEvent);
  }
}