#!/bin/bash
mkdir -p build
cd build
rm -rf *
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

if [ $? -eq 0 ]; then
    echo "============================================="
    echo "Build successful! Executable: timer_lab"
    echo
    echo "Auto-launching timer_lab..."
    echo "============================================="
    echo
    ./timer_lab
else
    echo "Build failed!"
fi
