#!/bin/bash
echo "[1/4] git pull..."
git pull

echo "[2/4] compile gcc..."
mkdir -p build
g++ -std=c++17 -Wall -o build/hello src/main.cpp
if [ $? -ne 0 ]; then
    exit 1
fi

echo "[3/4] run..."
./build/hello

echo "[4/4] OK!"
