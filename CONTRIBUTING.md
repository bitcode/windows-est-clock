# Contributing to EST Clock

Thank you for your interest in contributing to the EST Clock project! This document provides guidelines and instructions for contributing to the project.

## Development Environment Setup

### Prerequisites
- Windows 10/11
- g++ compiler from WinLibs (MinGW-w64)
- GDI32 libraries (included with Windows)

### Installing the g++ Compiler
You **must** install the g++ compiler from https://winlibs.com/ to build this project:

1. Visit [WinLibs.com](https://winlibs.com/)
2. Download the latest MinGW-w64 release (with GCC and LLVM/Clang/LLD/LLDB)
   - Choose the **Win64** version for 64-bit Windows
   - Select the **UCRT** runtime
   - Download the .zip archive (e.g., `winlibs-x86_64-posix-seh-gcc-13.2.0-llvm-17.0.6-mingw-w64ucrt-11.0.1-r1.zip`)
3. Extract the archive to a location on your system (e.g., `C:\mingw64`)
4. Add the bin directory to your PATH:
   - Right-click on "This PC" or "My Computer" and select "Properties"
   - Click on "Advanced system settings"
   - Click on "Environment Variables"
   - Under "System variables", find and select "Path", then click "Edit"
   - Click "New" and add the path to the bin directory (e.g., `C:\mingw64\bin`)
   - Click "OK" on all dialogs to save changes

## Building the Application

### Compilation Commands

```bash
# First compile the resource file
C:\mingw64\bin\windres resource.rc -O coff -o resource.res

# Then compile and link everything together
C:\mingw64\bin\g++.exe -O2 -DUNICODE -D_UNICODE main.cpp resource.res -o est_clock.exe -lgdi32 -lshell32 -lcomctl32 -mwindows
```

### Debug Build

For a debug build with symbols:

```bash
# First compile the resource file
C:\mingw64\bin\windres resource.rc -O coff -o resource.res

# Then compile with debug symbols
C:\mingw64\bin\g++.exe -g -DUNICODE -D_UNICODE main.cpp resource.res -o est_clock_debug.exe -lgdi32 -lshell32 -lcomctl32
```

## Debugging

### Using GDB

To debug using GDB:

```bash
gdb est_clock_debug.exe
```

### Using Visual Studio Code

The project includes a `.vscode/launch.json` file with preconfigured debug settings for:
- Launching the application with GDB
- Attaching to a running process

## Code Style Guidelines

Please follow these guidelines when contributing code:

1. **Indentation**: Use 4 spaces for indentation, not tabs.
2. **Naming Conventions**:
   - Use `g_` prefix for global variables (e.g., `g_hwnd`)
   - Use `WM_` prefix for Windows message constants
   - Use `IDM_` prefix for menu command identifiers
3. **Comments**: Add comments for complex logic and function descriptions.
4. **Error Handling**: Always check return values from Windows API functions and handle errors appropriately.
5. **Memory Management**: Properly clean up resources (icons, fonts, etc.) in the `WM_DESTROY` handler.

## Pull Request Process

1. Fork the repository and create a new branch for your feature or bug fix.
2. Make your changes, following the code style guidelines.
3. Test your changes thoroughly.
4. Update documentation if necessary.
5. Submit a pull request with a clear description of the changes.
6. Ensure all checks pass.

## Architecture Overview

The EST Clock application consists of several key components:

1. **Window Manager**: Handles window creation, styling, and lifecycle.
2. **Time Manager**: Fetches UTC time and converts it to Eastern Time.
3. **Rendering Engine**: Draws the formatted time string.
4. **System Tray Integration**: Manages the system tray icon and context menu.
5. **Format Manager**: Handles switching between 12-hour and 24-hour formats.

For more detailed architectural information, refer to the documentation in the `docs` directory.

## License

By contributing to this project, you agree that your contributions will be licensed under the same license as the project (see LICENSE file).