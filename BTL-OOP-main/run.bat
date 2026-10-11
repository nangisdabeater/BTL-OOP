@echo off
title Quan Ly Trung Tam CSKH
cd /d "%~dp0"
if exist "build\myprogram.exe" (
    build\myprogram.exe
) else if exist "myprogram.exe" (
    myprogram.exe
) else (
    echo Khong tim thay myprogram.exe. Dang bien dich lai...
    cmake -B build -G "MinGW Makefiles"
    cmake --build build
    build\myprogram.exe
)
pause
