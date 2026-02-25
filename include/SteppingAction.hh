#ifndef TyOneSteppingAction_h
#define TyOneSteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "globals.hh"

class G4LogicalVolume;

namespace tyone
{

  class EventAction;

  class SteppingAction : public G4UserSteppingAction
  {
  public:
    SteppingAction();
    ~SteppingAction() override;

    void UserSteppingAction(const G4Step *) override;
  };

}

#endif
