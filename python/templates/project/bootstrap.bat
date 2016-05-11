:: Run this first on a clean system to prepare <<PROJECT_NAME>> build.

@ECHO OFF
SETLOCAL

:: Change to root dir
CD /d %~dp0

:: Make sure we're in a clean state
call .\clean.bat
if %errorlevel% neq 0 exit /b %errorlevel%

:: Clone gaen if it doesn't exist. This will need to happen for
:: anyone who has cloned the project specific repository and is running
:: bootstrap.bat for the first time.
IF NOT EXIST .\gaen (
  git clone -b <<GAEN_GIT_BRANCH>> <<GAEN_GIT_REMOTE>> gaen
  if %errorlevel% neq 0 exit /b %errorlevel%
  cd gaen
  git checkout <<GAEN_GIT_COMMIT>>
  if %errorlevel% neq 0 exit /b %errorlevel%
  cd ..
)

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
   if %errorlevel% neq 0 exit /b %errorlevel%
)


:: Create our project specific system_api_meta.cpp
python "%~dp0\gaen\python\codegen_api.py"
if %errorlevel% neq 0 exit /b %errorlevel%

:: Issue cmake command
cd %BUILD_DIR%
if "%PLAT%"=="win64" (
    cmake -G "Visual Studio 12 Win64" %<<PROJECT_NAME_UPPER>>_ROOT%
    if %errorlevel% neq 0 exit /b %errorlevel%
)
if "%PLAT%"=="win32" (
    cmake -G "Visual Studio 12" %<<PROJECT_NAME_UPPER>>_ROOT%
    if %errorlevel% neq 0 exit /b %errorlevel%
)

:: Build cmpc so we can run codegen.py and boostrap our scripts
call "%VS120COMNTOOLS%\vsvars32.bat"
if %errorlevel% neq 0 exit /b %errorlevel%
msbuild "%BUILD_DIR%\gaen\src\cmpc\cmpc.vcxproj"
if %errorlevel% neq 0 exit /b %errorlevel%

:: Do the .cmp -> .cpp codegen
echo.
echo Running cmpc to compile Compose scripts...
python "%~dp0\gaen\python\codegen.py"
if %errorlevel% neq 0 exit /b %errorlevel%
echo.

:: Re-run cmake since the compiled scripts have been added to
:: the cmake files.
if "%PLAT%"=="win64" (
    cmake -G "Visual Studio 12 Win64" %<<PROJECT_NAME_UPPER>>_ROOT%
    if %errorlevel% neq 0 exit /b %errorlevel%
)
if "%PLAT%"=="win32" (
    cmake -G "Visual Studio 12" %<<PROJECT_NAME_UPPER>>_ROOT%
    if %errorlevel% neq 0 exit /b %errorlevel%
)

echo.
echo Bootstrapping complete.
echo Visual Studio solution: %BUILD_DIR%\<<PROJECT_NAME>>.sln
echo.

