:: Run this first on a clean system to prepare Gaen build.

@ECHO OFF
SETLOCAL

:: Change to root dir
CD /d %~dp0

:: Write root directory to main GAEN_ROOT env var
FOR /f "tokens=1" %%B in ('CHDIR') do set GAEN_ROOT=%%B

set BUILD_DIR=%GAEN_ROOT%\build\win64

if not exist "%BUILD_DIR%" (
   mkdir "%BUILD_DIR%"
)

cd %BUILD_DIR%

cmake -G "Visual Studio 12 Win64" %GAEN_ROOT%

