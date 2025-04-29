@echo off
echo ===================================================
echo EST-Clock Test Runner with Time Resynchronization
echo ===================================================

echo.
echo Setting up test environment...

REM Store the current directory
set CURRENT_DIR=%CD%

REM Navigate to the build directory if it exists, or create it
if exist build (
    cd build
) else (
    mkdir build
    cd build
)

echo.
echo Configuring tests with CMake...
cmake -S .. -B .

echo.
echo Building tests...
cmake --build .

echo.
echo Running tests...
echo ---------------------------------------------------
ctest -C Debug -V

echo.
echo ---------------------------------------------------
echo Ensuring time synchronization after tests...
echo ---------------------------------------------------

REM Run time resynchronization regardless of test outcome
echo Resynchronizing system time...
w32tm /resync

if %ERRORLEVEL% neq 0 (
    echo WARNING: Failed to resynchronize time. Error code: %ERRORLEVEL%
    echo Attempting alternative resynchronization method...
    
    REM Try an alternative method to resync time
    net time /set
    
    if %ERRORLEVEL% neq 0 (
        echo WARNING: Alternative time resynchronization also failed.
        echo Please manually run 'w32tm /resync' in an administrator PowerShell.
    ) else (
        echo Alternative time resynchronization completed successfully.
    )
) else (
    echo Time resynchronization completed successfully.
)

REM Return to the original directory
cd %CURRENT_DIR%

echo.
echo ===================================================
echo Test execution completed
echo ===================================================