# Windows EST Clock

A simple desktop clock application displaying Eastern Time (ET) with clean visuals and minimal system impact.

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
- Lightweight native implementation (no Electron/bloat)
- Low CPU/memory usage
- Simple, distraction-free interface
- System tray icon for easy access
- Toggle between 12-hour and 24-hour time formats
- Draggable window positioning
- Semi-transparent overlay stays on top of other windows

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

## Advanced Usage

- Right-click the clock window for the same options as the tray icon
- The clock automatically adjusts for daylight saving time changes
- The semi-transparent window stays on top of other applications

## Project Structure
- `main.cpp` - Main application code
- `resource.h` and `resource.rc` - Resource definitions
- `windows-est-clock.ico` - Application icon
- `build.bat` - Build script
- `docs/` - Documentation
  - `ARCHITECTURE.md` - Detailed architecture documentation

## Contributing
Contributions are welcome! Please see [CONTRIBUTING.md](CONTRIBUTING.md) for detailed guidelines.

All contributors are expected to follow our [Code of Conduct](CODE_OF_CONDUCT.md).

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