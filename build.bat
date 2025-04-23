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
