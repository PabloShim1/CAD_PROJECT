#ifndef TyOneSteppingAction_h
#define TyOneSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

namespace tyone
{
  class SteppingAction : public G4UserSteppingAction
  {
  public:
    SteppingAction();
    ~SteppingAction() override = default;

    void UserSteppingAction(const G4Step*) override;
  };
}

#endif