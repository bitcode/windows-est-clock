@echo off
echo Building EST Clock...

REM Create build directory if it doesn't exist
if not exist build mkdir build

echo Compiling resource file...
C:\mingw64\bin\windres resource.rc -O coff -o build\resource.res
if %ERRORLEVEL% NEQ 0 (
    echo Resource compilation failed with error code %ERRORLEVEL%
    exit /b %ERRORLEVEL%
)

echo Compiling release build...

REM Check if the output file exists and try to delete it first
if exist build\windows-est-clock-portable.exe (
    echo Output file exists, attempting to remove it...
    del /F /Q build\windows-est-clock-portable.exe
    if exist build\windows-est-clock-portable.exe (
        echo Warning: Could not remove existing executable. It may be in use.
        echo Please close any running instances of the application and try again.
        tasklist | findstr windows-est-clock-portable.exe
        exit /b 1
    )
)

C:\mingw64\bin\g++.exe -O2 -DUNICODE -D_UNICODE main.cpp build\resource.res -o build\windows-est-clock-portable.exe -lgdi32 -lshell32 -lcomctl32 -mwindows
if %ERRORLEVEL% NEQ 0 (
    echo Release build failed with error code %ERRORLEVEL%
    exit /b %ERRORLEVEL%
)

echo Compiling debug build...
C:\mingw64\bin\g++.exe -g -DUNICODE -D_UNICODE main.cpp build\resource.res -o build\est_clock_debug.exe -lgdi32 -lshell32 -lcomctl32
if %ERRORLEVEL% NEQ 0 (
    echo Debug build failed with error code %ERRORLEVEL%
    exit /b %ERRORLEVEL%
)

echo Build completed successfully!
echo Release build: build\windows-est-clock-portable.exe
echo Debug build: build\est_clock_debug.exe
