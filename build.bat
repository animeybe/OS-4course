@echo off
mkdir build 2>nul
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
mkdir logs 2>nul
cd ..
build\Release\temp_logger.exe
