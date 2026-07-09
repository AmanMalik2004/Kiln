@echo off
g++ -o minic.exe compiler.cpp
minic.exe test2.mc
nasm -f win32 output.asm -o output.obj
gcc output.obj -o output.exe
output.exe
pause
