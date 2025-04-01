# Windows EST Clock

A native Windows desktop clock application displaying Eastern Time (ET) with clean visuals and minimal system impact.

## Features
- Accurate Eastern Time display (automatically adjusts for daylight saving)
- Lightweight native implementation (no Electron/bloat)
- Low CPU/memory usage
- Simple, distraction-free interface
- System tray icon for easy access
- Toggle between 12-hour and 24-hour time formats
- Draggable window positioning
- Semi-transparent overlay stays on top of other windows
## Installation (Compiling from Source)

### Prerequisites
- Windows 10/11
- g++ compiler from WinLibs (MinGW-w64)
- GDI32 libraries (included with Windows)

#### Installing the g++ Compiler
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
   git clone https://github.com/bitcode/EST-clock.git
   cd EST-clock
   ```
2. Compile the application:
   ```powershell
   g++ -g -DUNICODE -D_UNICODE main.cpp -o est_clock.exe -lgdi32
   ```
   
   For a release build with optimizations:
   ```powershell
   g++ -O2 -DUNICODE -D_UNICODE main.cpp -o est_clock.exe -lgdi32
   ```
3. Run the executable:
   ```powershell
   .\est_clock.exe
   ```
   ```
## Usage
- The application displays a transparent clock showing Eastern Time
- The clock window can be dragged to any position on the screen
- The application runs in the system tray
- Left-click the tray icon to show/hide the clock window
- Right-click the tray icon for options:
  - Toggle between 12-hour and 24-hour time formats
  - Exit application
- Right-click the clock window for the same options
  - Exit application

## Future Plans
- [ ] Create pre-built .exe installer
- [ ] Add MSI package for enterprise deployment
- [ ] Implement additional timezone support
- [ ] Add customization options (themes, colors, font sizes)
- [ ] Add opacity control
- [ ] Support for multiple monitor setups

## Contributing
Pull requests welcome! Please ensure:
- Code follows existing style
- Changes are Windows-compatible
- No external dependencies are introduced
- All code compiles with the g++ compiler from WinLibs

## Debugging
For debugging, compile with debug symbols:
```powershell
g++ -g -DUNICODE -D_UNICODE main.cpp -o main_debug.exe -lgdi32
```

You can use GDB for debugging:
```powershell
gdb main_debug.exe
```