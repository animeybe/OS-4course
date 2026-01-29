@echo off
echo [1] Git setup...
git checkout lab2-process 2>nul || git checkout -b lab2-process
git pull origin lab2-process 2>nul || echo Skip remote

echo [2] Clean...
rmdir /s /q build 2>nul
mkdir build
cd build

echo [3] Tools...
gcc --version
cmake --version
make --version

echo [4] CMake...
cmake .. -G "MinGW Makefiles" -DCMAKE_MAKE_PROGRAM=make.exe

echo [5] Build...
make

echo [6] Test...
if exist process_test.exe (
    process_test.exe
) else (
    echo ERROR: process_test.exe not found
    dir *.exe
)

cd ..
pause
