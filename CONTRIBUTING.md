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

## Testing

### Running Tests

The project includes comprehensive tests for time conversion functionality. The easiest way to run all tests is using the provided test runner script:

```powershell
cd tests
.\run_tests_with_resync.ps1
```

This script will:
- Configure the tests with CMake
- Build all test executables
- Run all tests using CTest
- Resynchronize the system time after tests

Alternatively, you can build and run tests manually:

```powershell
cd tests
mkdir -p build && cd build
cmake ..
cmake --build . --config Debug
ctest -C Debug -V
```

### Test Structure

- `time_conversion_tests.cpp`: Basic time conversion tests
- `time_zone_conversion_tests.cpp`: Tests for different time zone scenarios
- `system_time_zone_tests.cpp`: Tests for system time zone changes
- `globals.cpp`: Global variables for testing
- `time_conversion_mock.cpp`: Mock implementations for testing
- `run_tests_with_resync.ps1`: PowerShell script to run tests with time resynchronization
- `run_tests_with_resync.bat`: Batch script to run tests with time resynchronization

### Administrator Privileges for System Time Zone Tests

Some tests in `system_time_zone_tests.cpp` require administrator privileges to modify system time and time zone settings. When running these tests:

1. Run PowerShell or Command Prompt as Administrator
2. Navigate to the test directory
3. Run the tests using the test runner script or directly

If you run these tests without administrator privileges, they will automatically skip the tests that require system changes and display a warning message.

### Adding New Tests

When adding new functionality, please include appropriate tests:

1. For basic functionality, add tests to `time_conversion_tests.cpp`
2. For time zone specific functionality, add tests to `time_zone_conversion_tests.cpp`
3. For tests that require system changes, add to `system_time_zone_tests.cpp`
4. Make sure to handle the case where administrator privileges are not available
5. If needed, update the mock implementations in `time_conversion_mock.cpp`

For more detailed information about the test suite, see [tests/README.md](tests/README.md).

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

## Branch Strategy

We follow a structured branching strategy:

- `main`: Production-ready code
- `develop`: Integration branch for features
- `feature/*`: Feature branches
- `bugfix/*`: Bug fix branches
- `release/*`: Release preparation branches

## Pull Request Process

1. Fork the repository and create a new branch for your feature or bug fix.
   ```bash
   git checkout -b feature/amazing-feature
   ```
2. Make your changes, following the code style guidelines.
3. Test your changes thoroughly, including running the time conversion tests.
   ```powershell
   cd tests
   .\run_tests_with_resync.ps1
   ```
4. Update documentation if necessary.
5. Commit your changes with a descriptive message.
   ```bash
   git commit -m "Add amazing feature"
   ```
6. Push to your fork.
   ```bash
   git push origin feature/amazing-feature
   ```
7. Submit a pull request with a clear description of the changes.
8. Ensure all automated checks pass.

## Continuous Integration and Deployment

This project uses GitHub Actions for CI/CD:

### Pull Request Workflow
- Triggered on pull requests to the main branch
- Builds the application
- Runs all tests
- Reports build and test status

### Main Branch Workflow
- Triggered on pushes to the main branch
- Builds the application
- Runs all tests
- Creates a pre-release with the latest executable

### Release Workflow
- Triggered when a tag is pushed (v*.*.*)
- Builds the application
- Runs all tests
- Creates an official release with the versioned executable

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