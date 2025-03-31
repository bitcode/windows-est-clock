# EST Clock Application Commands

## Compilation
To compile the application from source (main.cpp):

Simple compilation:
```bash
g++ main.cpp -o main_debug.exe
```

Detailed compilation with debug symbols and Unicode support:
```powershell
C:\mingw64\bin\g++.exe -g -DUNICODE -D_UNICODE main.cpp -o main_debug.exe -lgdi32 ; "(Roo/PS Workaround: 9)" > $null; start-sleep -milliseconds 150
```

For a release build with optimizations:
```bash
g++ main.cpp -O2 -o est_clock.exe
```

## Running
To run the debug executable:
```bash
./main_debug.exe
```

To run the release executable:
```bash
./est_clock.exe
```

## Debugging
To debug using gdb:
```bash
gdb main_debug.exe
```

## VSCode Configuration
The project includes a `.vscode/launch.json` file with preconfigured debug settings for:
- Launching the application with gdb
- Attaching to a running process