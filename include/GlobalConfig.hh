#ifndef GlobalConfig_h
#define GlobalConfig_h 1

#include "G4SystemOfUnits.hh"
#include "globals.hh"

namespace tyone {
    // === ВЫБОР МЕТОДА ДЕТЕКТИРОВАНИЯ ===
    // 1 - Scoring Mesh (Воксельная 3D сетка). 
    //     Идеально для твоего Jupyter (создает гистограмму для анализа распределения доз).
    // 2 - Sensitive Detector (Точечная запись). 
    //     Для записи каждого отдельного взаимодействия (x, y, z, edep) в дерево.
    const G4int SCORING_METHOD = 1; 

    // === НАСТРОЙКИ СЕТКИ (Для метода 1) ===
    // Шаг сетки (разрешение одного вокселя).
    // Чем меньше шаг, тем красивее картинка в Jupyter, но дольше расчет.
    const G4double MESH_STEP = 2.0 * mm;

    // Отступ от краев модели. Сетка будет на 2 см шире модели с каждой стороны,
    // чтобы захватить "рассеяние" в воздухе вокруг.
    const G4double MESH_PADDING = 2.0 * cm; 

    // === ВЫВОД ДАННЫХ ===
    // Имя выходного файла. Geant4 сам добавит .root.
    // Твой анализ в Python будет открывать именно этот файл.
    const G4String OUTPUT_FILE = "results/glove_dose_map";

    // Флаг для записи в ROOT формат (нужен для библиотеки uproot в твоем анализе)
    const G4bool USE_ROOT = true;
}

#endif