@echo off
cd /d "%~dp0"
if "%~1"=="" (echo Usage: run.bat 1..7 & exit /b 1)
call build.bat %1
if errorlevel 1 exit /b 1
set "EXE=bin\lab2_2_%1\Debug\lab2_2_%1.exe"
"%EXE%"
