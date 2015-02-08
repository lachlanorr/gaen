:: Run this first on a clean system to prepare <<PROJECT_NAME>> build.

@ECHO OFF
SETLOCAL

:: Change to root dir
CD /d %~dp0

:: Write root directory to main <<PROJECT_NAME_UPPER>>_ROOT env var
FOR /f "tokens=1" %%B in ('CHDIR') do set <<PROJECT_NAME_UPPER>>_ROOT=%%B

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

set BUILD_DIR=%<<PROJECT_NAME_UPPER>>_ROOT%\build\%PLAT%

if not exist "%BUILD_DIR%" (
   mkdir "%BUILD_DIR%"
)

cd %BUILD_DIR%

if "%PLAT%"=="win64" (
    cmake -G "Visual Studio 12 Win64" %<<PROJECT_NAME_UPPER>>_ROOT%
)
if "%PLAT%"=="win32" (
    cmake -G "Visual Studio 12" %<<PROJECT_NAME_UPPER>>_ROOT%
)


:: Build cmpc so we can run codegen.py and boostrap our scripts
call "%VS120COMNTOOLS%\vsvars32.bat"
msbuild "%BUILD_DIR%\gaen\src\cmpc\cmpc.vcxproj"

:: Do the .cmp -> .cpp codegen
python "%~dp0\gaen\python\codegen.py"

:: Re-run cmake since the compiled scripts have been added to
:: the cmake files.
if "%PLAT%"=="win64" (
    cmake -G "Visual Studio 12 Win64" %<<PROJECT_NAME_UPPER>>_ROOT%
)
if "%PLAT%"=="win32" (
    cmake -G "Visual Studio 12" %<<PROJECT_NAME_UPPER>>_ROOT%
)

echo.
echo Bootstrapping complete.
echo Visual Studio solution: %BUILD_DIR%\<<PROJECT_NAME>>.sln
echo.

