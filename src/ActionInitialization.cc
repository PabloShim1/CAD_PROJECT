#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"

namespace tyone
{
  ActionInitialization::ActionInitialization() : G4VUserActionInitialization() {}
  ActionInitialization::~ActionInitialization() {}

  void ActionInitialization::BuildForMaster() const
  {
    // RunAction нужен мастеру для управления файлом в многопоточном режиме
    SetUserAction(new RunAction());
  }

  void ActionInitialization::Build() const
  {
    // Регистрация генератора частиц
    SetUserAction(new PrimaryGeneratorAction());

    // Регистрация управления файлом (открытие/запись)
    SetUserAction(new RunAction());

    // Регистрация сбора данных на каждом шаге
    SetUserAction(new SteppingAction());
  }
}