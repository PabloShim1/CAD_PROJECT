#include "PrimaryGeneratorAction.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"
#include "G4RunManager.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

namespace tyone
{
  PrimaryGeneratorAction::PrimaryGeneratorAction()
  {
    this->fGPS = new G4GeneralParticleSource();
  }

  PrimaryGeneratorAction::~PrimaryGeneratorAction()
  {
    delete this->fGPS;
  }

  void PrimaryGeneratorAction::GeneratePrimaries(G4Event *anEvent)
  {
    this->fGPS->GeneratePrimaryVertex(anEvent);
  }
}
