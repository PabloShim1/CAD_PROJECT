#ifndef TyOnePrimaryGeneratorAction_h
#define TyOnePrimaryGeneratorAction_h 1

#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4GeneralParticleSource.hh"
#include "globals.hh"

class G4GeneralParticleSource;
class G4Event;
class G4Box;

namespace tyone
{

  class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
  {
  public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction() override;

    void GeneratePrimaries(G4Event *) override;

  private:
    G4GeneralParticleSource *fGPS = nullptr;
  };

}

#endif
