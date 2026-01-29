#!/bin/bash
echo "Updating repository..."
git pull origin lab2-process 2>nul || echo "Skip remote"

echo "Cleaning previous build..."
rm -rf build

echo "Configuring with CMake..."
mkdir -p build && cd build
cmake ..

echo "Building..."
make

echo "Running test..."
if [ -f process_test ]; then
    ./process_test
else
    echo "ERROR: process_test not found"
    ls -la *.exe process_test 2>/dev/null || ls
fi

cd ..
