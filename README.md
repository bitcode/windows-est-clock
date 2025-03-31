# Windows EST Clock

A native Windows desktop clock application displaying Eastern Standard Time (EST) with clean visuals and minimal system impact.

## Features
- Accurate EST time display (automatically adjusts for daylight saving)
- Lightweight native implementation (no Electron/bloat)
- Low CPU/memory usage
- Simple, distraction-free interface

## Installation (Compiling from Source)

### Prerequisites
- Windows 10/11
- MinGW-w64 (g++ compiler)
- GDI32 libraries (included with Windows)

### Build Steps
1. Clone this repository:
   ```powershell
   git clone https://github.com/[your-username]/windows-est-clock.git
   cd windows-est-clock
   ```
2. Compile the application:
   ```powershell
   C:\mingw64\bin\g++.exe -g -DUNICODE -D_UNICODE main.cpp -o est_clock.exe -lgdi32
   ```
3. Run the executable:
   ```powershell
   .\est_clock.exe
   ```

## Usage
- The application runs in the system tray by default
- Right-click the tray icon for options:
  - Show/Hide main window
  - Exit application

## Future Plans
- [ ] Create pre-built .exe installer
- [ ] Add MSI package for enterprise deployment
- [ ] Implement additional timezone support
- [ ] Add customization options (themes, formats)

## Contributing
Pull requests welcome! Please ensure:
- Code follows existing style
- Changes are Windows-compatible
- No external dependencies are introduced