#ifndef TyOneActionInitialization_h
#define TyOneActionInitialization_h 1

#include "G4VUserActionInitialization.hh"

namespace tyone
{

  class ActionInitialization : public G4VUserActionInitialization
  {
  public:
    ActionInitialization();
    ~ActionInitialization() override;

    void BuildForMaster() const override;
    void Build() const override;
  };

}

#endif
