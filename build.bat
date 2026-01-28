@echo off
echo [1/4] git pull...
git pull

echo [2/4] compile MinGW...
if not exist build mkdir build
C:\mingw\bin\g++ -std=c++17 -Wall -o build\hello.exe src\main.cpp
if %errorlevel% neq 0 exit /b %errorlevel%

echo [3/4] run...
build\hello.exe

echo [4/4] OK!
pause
