@echo off
setlocal EnableExtensions

set "VCVARS=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
if not exist "%VCVARS%" (
  set "VCVARS=C:\Program Files\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
)
if not exist "%VCVARS%" (
  echo [ERROR] Не знайдено vcvars64.bat. Встановіть "Desktop development with C++" у Visual Studio.
  exit /b 1
)

cd /d "%~dp0"
call "%VCVARS%" >nul

if "%~1"=="" (
  echo Використання: build.bat lab1_2_3.cpp
  echo Або:          build.bat 3
  echo Або:          build.bat all
  exit /b 1
)

set "ARG=%~1"
if /I "%ARG%"=="all" goto :build_all

echo %ARG%| findstr /R "^[1-7]$" >nul
if not errorlevel 1 set "ARG=lab1_2_%ARG%.cpp"

set "SRC=%ARG%"
if not exist "%SRC%" (
  echo [ERROR] Файл не знайдено: %SRC%
  exit /b 1
)

for %%I in ("%SRC%") do (
  if not exist "bin\%%~nI\Debug" mkdir "bin\%%~nI\Debug"
  cl /nologo /EHsc /std:c++17 /utf-8 "%%~nxI" /Fe:"bin\%%~nI\Debug\%%~nI.exe" /Fo:"bin\%%~nI\Debug\\"
)
exit /b %ERRORLEVEL%

:build_all
set FAILED=0
for %%F in (1 2 3 4 5 6 7) do (
  call "%~f0" %%F
  if errorlevel 1 set FAILED=1
)
exit /b %FAILED%
