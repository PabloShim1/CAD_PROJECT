#!/bin/bash
PROJECT_ROOT=$(pwd)
G4LIB="$PROJECT_ROOT/third_party/geant4/lib"

export DYLD_LIBRARY_PATH="$G4LIB:$DYLD_LIBRARY_PATH"

echo "🛠 Собираем..."
# Мы не делаем clean, чтобы не ждать долго, если не надо
bazel build :cadgeant4

if [ $? -eq 0 ]; then
    cp init_vis.mac vis.mac run.mac glove.obj bazel-bin/ 2>/dev/null
    echo "🚀 Запуск..."
    cd bazel-bin && ./cadgeant4
else
    echo "❌ Ошибка линковки."
    echo "Давай проверим список библиотек:"
    ls $G4LIB | grep G4vis
    ls $G4LIB | grep G4UI
fi
