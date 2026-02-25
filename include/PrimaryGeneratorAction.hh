#ifndef TyOnePrimaryGeneratorAction_h
#define TyOnePrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4ParticleGun.hh"
#include "G4ThreeVector.hh"
#include "globals.hh"

class G4Event;

namespace tyone
{
  class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
  {
  public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction() override;

    // Метод генерации первичных частиц, вызывается каждый эвент
    void GeneratePrimaries(G4Event*) override;

  private:
    // Указатель на пушку частиц
    G4ParticleGun* fParticleGun = nullptr;

    // Переменные для хранения параметров источника
    // (Подтягиваются из DetectorConstruction в конструкторе или методе генерации)
    G4double size_x;
    G4double size_y;
    G4double source_x;
    G4double source_y;
    G4double source_z;
  };
}

#endif