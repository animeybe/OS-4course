@echo off
mkdir build 2>nul
cd build
del /Q *.* 2>nul
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
mingw32-make -j4

if %ERRORLEVEL% == 0 (
    echo.
    echo ========================================
    echo Build successful! Executable: temp_logger.exe
    echo.
    echo Raw log:    logs/raw_temps.log (24h)
    echo Hourly log: logs/hourly_avg.log (1 month)
    echo Daily log:  logs/daily_avg.log (1 year)
    echo ========================================
    echo.
    echo Creating logs directory...
    if not exist ..\logs mkdir ..\logs
    cd ..
    echo.
    echo Press any key to start Temperature Logger...
    pause >nul
    build\temp_logger.exe
) else (
    echo Build failed!
)
pause
