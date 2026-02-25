#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"

#include "G4RunManagerFactory.hh"
#include "G4SteppingVerbose.hh"
#include "G4UImanager.hh"
#include "QBBC.hh"

#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

// Принудительно подключаем заголовки драйверов
#include "G4OpenGLStoredQt.hh"
#include "G4OpenGLImmediateQt.hh"

#include "Randomize.hh"
#include "G4ScoringManager.hh"

using namespace tyone;

int main(int argc, char **argv)
{
    // 1. Инициализация UI (для интерактивного режима)
    G4UIExecutive *ui = nullptr;
    if (argc == 1)
    {
        ui = new G4UIExecutive(argc, argv);
    }

    // 2. Настройка VisManager с гарантированной регистрацией драйверов
    // Мы создаем их через 'new', чтобы линковщик не выкинул код из сборки
    G4VisManager *visManager = new G4VisExecutive;
    
    // Ручная регистрация (именно эти имена нужно использовать в макросах)
    visManager->RegisterGraphicsSystem(new G4OpenGLStoredQt);
    visManager->RegisterGraphicsSystem(new G4OpenGLImmediateQt);
    
    visManager->Initialize();

    // 3. Настройка ядра Geant4
    G4ScoringManager::GetScoringManager();

    auto *runManager =
        G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);

    runManager->SetUserInitialization(new DetectorConstruction());

    G4VModularPhysicsList *physicsList = new QBBC;
    physicsList->SetVerboseLevel(1);
    runManager->SetUserInitialization(physicsList);

    runManager->SetUserInitialization(new ActionInitialization());

    G4UImanager *UImanager = G4UImanager::GetUIpointer();

    if (!ui)
    {
        // Пакетный режим
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    }
    else
    {
        // Интерактивный режим
        // Согласно твоим правилам, используем только эти файлы:
        // 1. init_vis.mac
        // 2. vis.mac
        // 3. run.mac
        
        // Сначала принудительно пробуем открыть зарегистрированную систему
        UImanager->ApplyCommand("/vis/open OpenGLStoredQt");
        
        // Загружаем макрос инициализации
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        
        ui->SessionStart();
        delete ui;
    }

    // 4. Очистка памяти
    delete visManager;
    delete runManager;

    return 0;
}