@echo off
setlocal EnableExtensions
cd /d "%~dp0"

if "%~1"=="" (
  echo Запуск одного пункту лабораторної:
  echo   run.bat 1   ... lab1_2_1.cpp
  echo   run.bat 3   ... lab1_2_3.cpp
  echo   run.bat 7   ... lab1_2_7.cpp
  exit /b 1
)

set "N=%~1"
echo %N%| findstr /R "^[1-7]$" >nul
if errorlevel 1 (
  echo [ERROR] Номер має бути від 1 до 7.
  exit /b 1
)

set "NAME=lab1_2_%N%"
set "EXE=bin\%NAME%\Debug\%NAME%.exe"

call build.bat %N%
if errorlevel 1 exit /b 1

if not exist "%EXE%" (
  echo [ERROR] Не знайдено: %EXE%
  exit /b 1
)

echo.
echo ===== %NAME% =====
"%EXE%"
exit /b %ERRORLEVEL%
