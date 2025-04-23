@echo off
echo ===================================================
echo         EST Clock Release Creation Tool
echo ===================================================
echo.

REM Check if git is installed
git --version > nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Git is not installed or not in your PATH.
    echo Please install Git from https://git-scm.com/downloads
    exit /b 1
)

REM Check if we're in a git repository
git rev-parse --is-inside-work-tree > nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Not in a git repository.
    echo Please run this script from the root of your EST Clock repository.
    exit /b 1
)

REM Check for uncommitted changes
git diff-index --quiet HEAD --
if %ERRORLEVEL% NEQ 0 (
    echo WARNING: You have uncommitted changes in your repository.
    echo It's recommended to commit all changes before creating a release.
    echo.
    set /p CONTINUE="Do you want to continue anyway? (y/n): "
    if /i not "%CONTINUE%"=="y" exit /b 0
    echo.
)

REM Check if the build directory exists
if not exist build (
    echo Build directory not found. Creating it...
    mkdir build
)

REM Build the application
echo Building the application...
call build.bat

REM Check if the build was successful
if not exist build\windows-est-clock-portable.exe (
    echo ERROR: Build failed! Executable not found.
    echo Please check the build output for errors.
    exit /b 1
)
echo Build successful!
echo.

REM Ask for version number
set /p VERSION="Enter version number (e.g., 1.0.0): "

REM Validate version number format (basic check)
if "%VERSION%"=="" (
    echo ERROR: Version number cannot be empty.
    exit /b 1
)

REM Manual version validation
set "VALID=true"

REM Split the version into parts
for /f "tokens=1-3 delims=." %%a in ("%VERSION%") do (
    set "MAJOR=%%a"
    set "MINOR=%%b"
    set "PATCH=%%c"
)

REM Check if any part is missing
if "%MAJOR%"=="" set "VALID=false"
if "%MINOR%"=="" set "VALID=false"
if "%PATCH%"=="" set "VALID=false"

REM Check if any part is not a number
echo %MAJOR% | findstr /r "^[0-9]*$" > nul || set "VALID=false"
echo %MINOR% | findstr /r "^[0-9]*$" > nul || set "VALID=false"
echo %PATCH% | findstr /r "^[0-9]*$" > nul || set "VALID=false"

if "%VALID%"=="false" (
    echo ERROR: Invalid version format. Please use semantic versioning (e.g., 1.0.0)
    echo You entered: "%VERSION%"
    exit /b 1
)

REM Check if tag already exists
git tag | findstr /C:"v%VERSION%" > nul
if %ERRORLEVEL% EQU 0 (
    echo ERROR: Tag v%VERSION% already exists!
    exit /b 1
)

REM Ask for release notes
echo.
echo Please enter release notes (press Enter twice when finished):
echo -----------------------------------------------------

set NOTES=
:loop
set /p LINE=""
if "%LINE%"=="" goto :endloop
set NOTES=%NOTES%%LINE%\n
goto :loop
:endloop

REM Create a tag with release notes
echo.
echo Creating git tag v%VERSION%...
git tag -a v%VERSION% -m "EST Clock v%VERSION%\n\n%NOTES%"

REM Push the tag
echo Pushing tag to remote repository...
git push origin v%VERSION%

if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Failed to push tag to remote repository.
    echo Please check your internet connection and repository permissions.
    exit /b 1
)

echo.
echo ===================================================
echo         Release Process Started Successfully!
echo ===================================================
echo.
echo The GitHub Actions workflow has been triggered and will:
echo  1. Build the application
echo  2. Create a release with tag v%VERSION%
echo  3. Attach the windows-est-clock-portable.exe file
echo.
echo You can monitor the progress at:
echo https://github.com/yourusername/EST-clock/actions
echo.
echo Once complete, the release will be available at:
echo https://github.com/yourusername/EST-clock/releases
echo.
echo Don't forget to update the version number in your code for the next release!
