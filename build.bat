@echo off
mkdir build 2>nul
cd build
del /Q *.* 2>nul
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
mingw32-make -j4

if %ERRORLEVEL% == 0 (
    echo.
    echo ========================================
    echo Build successful! Executable: timer_lab.exe
    echo.
    echo To test: timer_lab.exe
    echo Multiple instances: start cmd /k timer_lab.exe
    echo Logs: logs\timer.log
    echo ========================================
    echo.
    echo Press any key to test single instance...
    pause >nul
    timer_lab.exe
) else (
    echo Build failed!
)
pause
