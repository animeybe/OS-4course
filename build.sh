#!/bin/bash
mkdir -p build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j4
mkdir -p logs
cd ..
./build/temp_logger
