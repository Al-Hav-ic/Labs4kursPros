@echo off
setlocal EnableExtensions
set "VCVARS=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
if not exist "%VCVARS%" set "VCVARS=C:\Program Files\Microsoft Visual Studio\2022\BuildTools\VC\Auxiliary\Build\vcvars64.bat"
if not exist "%VCVARS%" (echo [ERROR] vcvars64.bat not found & exit /b 1)
cd /d "%~dp0"
call "%VCVARS%" >nul
if "%~1"=="" (echo Usage: build.bat 1..7 ^| all & exit /b 1)
if /I "%~1"=="all" (
  for %%F in (1 2 3 4 5 6 7) do call "%~f0" %%F
  exit /b 0
)
set "N=%~1"
set "NAME=lab2_2_%N%"
if not exist "%NAME%.cpp" (echo Not found: %NAME%.cpp & exit /b 1)
if not exist "bin\%NAME%\Debug" mkdir "bin\%NAME%\Debug"
cl /nologo /EHsc /std:c++17 /utf-8 "%NAME%.cpp" /Fe:"bin\%NAME%\Debug\%NAME%.exe" /Fo:"bin\%NAME%\Debug\\"
exit /b %ERRORLEVEL%
