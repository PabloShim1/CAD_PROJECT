#include "EventAction.hh"
#include "RunAction.hh"

#include "G4Event.hh"
#include "G4RunManager.hh"

namespace tyone
{
  EventAction::EventAction(RunAction *runAction)
      : fRunAction(runAction)
  {
  }

  EventAction::~EventAction()
  {
  }

  void EventAction::BeginOfEventAction(const G4Event *)
  {
  }

  void EventAction::EndOfEventAction(const G4Event *)
  {
  }
}
