:: Run this first on a clean system to prepare Gaen build.

@ECHO OFF
SETLOCAL

:: Change to root dir
CD /d %~dp0

:: Make sure we're in a clean state
call .\clean.bat
if %errorlevel% neq 0 exit /b %errorlevel%

:: Pull down our submodules
git submodule init
git submodule update --remote --recursive

:: Write root directory to main GAEN_ROOT env var
FOR /f "tokens=1" %%B in ('CHDIR') do set GAEN_ROOT=%%B

if "%1"=="" (
    SET PLAT=win64
)
if "%1"=="win64" (
    SET PLAT=win64
)
if "%1"=="win32" (
    SET PLAT=win32
)
if "%PLAT%"=="" (
    echo Invalid platform type "%1", must be win32 or win64
    exit /b 1
)

set BUILD_DIR=%GAEN_ROOT%\build\%PLAT%

if not exist "%BUILD_DIR%" (
   mkdir "%BUILD_DIR%"
   if %errorlevel% neq 0 exit /b %errorlevel%
)

:: Issue cmake command
cd %BUILD_DIR%
if "%PLAT%"=="win64" (
    cmake -G "Visual Studio 16" -A x64  %GAEN_ROOT%
    if %errorlevel% neq 0 exit /b %errorlevel%
)
if "%PLAT%"=="win32" (
    cmake -G "Visual Studio 16" -A Win32 %GAEN_ROOT%
    if %errorlevel% neq 0 exit /b %errorlevel%
)

set VSDIR=c:\Program Files (x86)\Microsoft Visual Studio\2019\Community

:: Build scripts for the first time to ensure codegen happens
:: before VS is loaded. Otherwise the first build in VS will
:: not compile the scripts (it will generate them though)
if not exist "%BUILD_DIR%\src\scripts\registration.cpp" (
  call "%VSDIR%\VC\Auxiliary\Build\vcvars64.bat"
  if %errorlevel% neq 0 exit /b %errorlevel%
  msbuild "%BUILD_DIR%\src\scripts\scripts.vcxproj"
  if %errorlevel% neq 0 exit /b %errorlevel%
)

echo.
echo Bootstrapping complete.
echo Visual Studio solution: %BUILD_DIR%\gaen.sln
echo.

