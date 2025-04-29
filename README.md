# Windows EST Clock

A simple desktop clock application displaying Eastern Time (ET) with clean visuals and minimal system impact. The clock works regardless of your local time zone, making it perfect for international teams coordinating with Eastern Time business hours.

![EST Clock Screenshot](docs/screenshot.png)

## Quick Start Guide (For All Users)

### Installation
No installation required! Simply download and run:

1. Download the latest `windows-est-clock-portable.exe` from the [Releases](https://github.com/yourusername/EST-clock/releases) page
2. Double-click the downloaded file to run it
3. The clock will appear on your screen and an icon will be added to your system tray

### Making Sure You Can See the System Tray Icon

**Important:** Windows may automatically hide the system tray icon. To make it visible:

1. Right-click on the Windows taskbar
2. Select "Taskbar settings"
3. Scroll down to "Notification area"
4. Click on "Select which icons appear on the taskbar"
5. Find "Eastern Time Clock Overlay" in the list and toggle it to "On"

![System Tray Settings](docs/system_tray_settings.png)

Alternatively, you can turn on "Always show all icons in the notification area" to display all system tray icons.

### Basic Usage
- Left-click the tray icon to show/hide the clock window
- Right-click the tray icon for options (12/24-hour format, exit)
- Click and drag the clock to position it anywhere on your screen

## Features
- Accurate Eastern Time display (automatically adjusts for daylight saving)
- Works correctly regardless of your local time zone
- Handles international date line crossing, DST transitions, and leap years
- Lightweight native implementation (no Electron/bloat)
- Low CPU/memory usage
- Simple, distraction-free interface
- System tray icon for easy access
- Toggle between 12-hour and 24-hour time formats
- Draggable window positioning
- Semi-transparent overlay stays on top of other windows
- Comprehensive test suite for time zone conversions
- Robust handling of time zone changes, DST transitions, and date boundary crossings

## Releases

The latest version of EST Clock is always available on the [Releases](https://github.com/yourusername/EST-clock/releases) page. Simply download the `windows-est-clock-portable.exe` file and run it.

### Creating a New Release

For project maintainers, a new release can be created using the included script:

```powershell
.\create_release.bat
```

This script will:
1. Build the application
2. Ask for a version number
3. Collect release notes
4. Create and push a Git tag
5. Trigger the GitHub Actions workflow to create the release

## Technical Details

EST Clock is a lightweight Windows application written in C++ using the Win32 API. It uses minimal system resources and has no external dependencies.

### How It Works
- The application creates a transparent, always-on-top window to display the time
- It uses the Windows API to retrieve the current UTC time and converts it to Eastern Time
- The system tray icon provides easy access to show/hide the clock and access settings
- The application automatically adjusts for daylight saving time changes
- Time zone conversion logic handles various edge cases including:
  - International date line crossing
  - Daylight Saving Time transitions
  - Month and year boundary changes
  - Different user local time zones

## For Developers: Building from Source

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

**Why this specific compiler is required:**
This project uses Windows-specific API functions and requires a compiler that properly supports the Windows API headers and libraries. The WinLibs distribution of MinGW-w64 provides the necessary Windows headers, libraries, and compiler flags to build native Windows applications. Using other compilers or versions may result in compilation errors or runtime issues.

### Build Steps
1. Clone this repository:
   ```powershell
   git clone https://github.com/yourusername/EST-clock.git
   cd EST-clock
   ```

2. Use the build script:
   ```powershell
   .\build.bat
   ```

   This will create both release and debug builds in the `build` directory.

3. Alternatively, compile manually:
   ```powershell
   # First compile the resource file
   C:\mingw64\bin\windres resource.rc -O coff -o resource.res

   # Then compile and link everything together
   C:\mingw64\bin\g++.exe -O2 -DUNICODE -D_UNICODE main.cpp resource.res -o est_clock.exe -lgdi32 -lshell32 -lcomctl32 -mwindows
   ```

4. Run the executable:
   ```powershell
   .\build\windows-est-clock-portable.exe
   ```

### Running Tests
The project includes comprehensive tests for time conversion functionality:

1. Navigate to the `tests` directory
2. Use the test runner script to build and run all tests:
   ```powershell
   .\run_tests_with_resync.ps1
   ```

   This script will:
   - Configure the tests with CMake
   - Build all test executables
   - Run all tests using CTest
   - Resynchronize the system time after tests

3. Alternatively, you can build and run tests manually:
   ```powershell
   mkdir -p build && cd build
   cmake ..
   cmake --build . --config Debug
   ctest -C Debug -V
   ```

For more detailed information about the test suite, see [tests/README.md](tests/README.md).

## Advanced Usage

- Right-click the clock window for the same options as the tray icon
- The clock automatically adjusts for daylight saving time changes
- The semi-transparent window stays on top of other applications

## Project Structure
- `main.cpp` - Main application code
- `resource.h` and `resource.rc` - Resource definitions
- `windows-est-clock.ico` - Application icon
- `build.bat` - Build script
- `tests/` - Test suite for time conversion functionality
  - `time_conversion_tests.cpp` - Basic time conversion tests
  - `time_zone_conversion_tests.cpp` - Tests for different time zone scenarios
  - `system_time_zone_tests.cpp` - Tests for system time zone changes
  - `globals.cpp` - Global variables for testing
  - `time_conversion_mock.cpp` - Mock implementations for testing
  - `run_tests_with_resync.ps1` - PowerShell script to run tests with time resynchronization
  - `run_tests_with_resync.bat` - Batch script to run tests with time resynchronization
  - `README.md` - Detailed documentation for the test suite
- `docs/` - Documentation
  - `ARCHITECTURE.md` - Detailed architecture documentation
- `.github/workflows/` - CI/CD pipeline configuration
  - `pull-request.yml` - Workflow for pull requests
  - `main.yml` - Workflow for main branch pushes
  - `release.yml` - Workflow for creating releases

## Contributing
Contributions are welcome! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for detailed guidelines.

All contributors are expected to follow our [Code of Conduct](CODE_OF_CONDUCT.md).

### GitHub Actions Workflows

This project uses GitHub Actions for automated building, testing, and releasing:

#### Pull Request Workflow
- Triggered on pull requests to the main branch
- Builds the application
- Runs all tests
- Reports build and test status

#### Main Branch Workflow
- Triggered on pushes to the main branch
- Builds the application
- Runs all tests
- Creates a pre-release with the latest executable

#### Release Workflow
- Triggered when a tag is pushed (v*.*.*)
- Builds the application
- Runs all tests
- Creates an official release with the versioned executable

### Branch Strategy
- `main`: Production-ready code
- `develop`: Integration branch for features
- `feature/*`: Feature branches
- `bugfix/*`: Bug fix branches
- `release/*`: Release preparation branches

## Debugging
For debugging, use the debug build:
```powershell
.\build\est_clock_debug.exe
```

You can use GDB for debugging:
```powershell
gdb .\build\est_clock_debug.exe
```

The project includes a `.vscode/launch.json` file with preconfigured debug settings.

## Future Plans
- [ ] Create pre-built .exe installer
- [ ] Add MSI package for enterprise deployment
- [ ] Implement additional timezone support
- [ ] Add customization options (themes, colors, font sizes)
- [ ] Add opacity control
- [ ] Support for multiple monitor setups

## Versioning

This project follows Semantic Versioning. For details on the versioning strategy, see [docs/VERSIONING.md](docs/VERSIONING.md).

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
