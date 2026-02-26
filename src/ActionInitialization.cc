#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"

namespace tyone
{

  ActionInitialization::ActionInitialization()
  {
  }

  ActionInitialization::~ActionInitialization()
  {
  }

  void ActionInitialization::BuildForMaster() const
  {
    // В многопоточном режиме Master следит за RunAction
    SetUserAction(new RunAction);
  }

  void ActionInitialization::Build() const
  {
    // 1. Генерация частиц
    SetUserAction(new PrimaryGeneratorAction);

    // 2. Управление файлами и гистограммами (ROOT)
    RunAction *runAction = new RunAction;
    SetUserAction(runAction);

    // 3. Обработка каждого шага частицы (ЗАПИСЬ ДОЗЫ)
    // Теперь это активно!
    SetUserAction(new SteppingAction);
  }

}