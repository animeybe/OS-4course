#!/bin/bash
mkdir -p build
cd build
rm -rf *
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j4

if [ $? -eq 0 ]; then
    echo ""
    echo "============================================"
    echo "Build successful! Executable: temp_logger"
    echo ""
    echo "Raw log:    logs/raw_temps.log (24h)"
    echo "Hourly log: logs/hourly_avg.log (1 month)"
    echo "Daily log:  logs/daily_avg.log (1 year)"
    echo "============================================"
    echo ""
    echo "Creating logs directory..."
    mkdir -p ../logs
    cd ..
    echo ""
    echo "Starting Temperature Logger... (Ctrl+C to stop)"
    echo ""
    ./build/temp_logger
else
    echo "Build failed!"
fi
