# EST Clock Architecture and System Design

## Overview

This document outlines the high-level architecture and system design for the EST Clock application, a Windows application that displays a transparent overlay clock showing the current Eastern Time Zone (ET), regardless of the user's system time zone settings.

## Goals

- Display the current Eastern Time (ET), including handling Daylight Saving Time (DST)
- Present the clock in a transparent, borderless window
- Ensure the clock window remains on top of other applications
- Update the displayed time periodically (every second)
- Utilize only standard Windows API functions
- Provide system tray icon for easy access and control
- Support both 12-hour and 24-hour time formats
- Allow dragging the window to any desired position

## High-Level Architecture

The application consists of several interacting components responsible for specific tasks:

1. **Window Manager:** Handles the creation, styling (transparency, topmost), positioning, and lifecycle of the application window.
2. **Time Manager:** Responsible for fetching the current Coordinated Universal Time (UTC) and converting it to Eastern Time (ET), correctly accounting for DST.
3. **Rendering Engine:** Draws the formatted time string onto the window surface.
4. **Update Mechanism:** Uses a system timer to trigger periodic updates of the time display.
5. **System Tray Integration:** Manages the system tray icon and its context menu for controlling the application.
6. **Format Manager:** Handles switching between 12-hour and 24-hour time formats.
7. **Main Application Logic:** Contains the main message loop (`WinMain`) and the window procedure (`WndProc`) to handle system messages and coordinate component interactions.

## Component Details

### Window Manager

- **Creation:** Uses `CreateWindowEx` with the following crucial styles:
  - `WS_EX_LAYERED`: Enables transparency effects
  - `WS_EX_TOPMOST`: Keeps the window above all non-topmost windows
  - `WS_EX_TOOLWINDOW`: Prevents the window from appearing in the taskbar
  - `WS_POPUP`: Creates a borderless window suitable for an overlay
- **Transparency:** Managed by `SetLayeredWindowAttributes`. Uses alpha blending (opacity) to create a semi-transparent window.
- **Positioning & Sizing:** Initially positioned at the bottom-right corner of the screen. Window width adjusts automatically based on the selected time format.
- **Draggability:** Implements custom `WM_NCHITTEST` handling to allow dragging the window by treating the client area as a caption.

### Time Manager

- **UTC Acquisition:** Uses `GetSystemTime` to retrieve the current time in UTC.
- **ET Conversion:** Uses `SystemTimeToTzSpecificLocalTime` with the "Eastern Standard Time" time zone information.
- **DST Handling:** Automatically handled by the Windows API based on the time zone rules.

### Rendering Engine

- **Trigger:** Primarily driven by the `WM_PAINT` message.
- **Drawing Context:** Uses `BeginPaint` and `EndPaint` for the device context.
- **Text Formatting:** Formats the time string based on the selected format (12-hour or 24-hour).
- **Text Rendering:** Uses `DrawText` to render the formatted time string.
- **Appearance:** Uses a bold font with white text on a transparent background.

### Update Mechanism

- **Timer Creation:** Uses `SetTimer` with a 1000ms interval.
- **Timer Events:** Handles `WM_TIMER` messages to update the time.
- **Redrawing:** Uses `InvalidateRect` to trigger a repaint after time updates.

### System Tray Integration

- **Initialization:** Creates and adds a system tray icon using `Shell_NotifyIcon`.
- **Icon Interaction:** Handles left-click (show/hide window) and right-click (display context menu) events.
- **Context Menu:** Provides options to toggle time format and exit the application.

### Format Manager

- **Toggle Mechanism:** Switches between 12-hour and 24-hour formats.
- **Window Adjustment:** Resizes and repositions the window when changing formats.
- **Format Application:** Applies the selected format during rendering.

## Message Flow

The application follows the standard Windows message-driven architecture:

1. **Initialization:**
   - Register window class, create window
   - Create and select font
   - Create timer
   - Initialize system tray icon
   - Perform initial time update

2. **Message Loop:** Wait for and dispatch messages

3. **On Timer Tick (`WM_TIMER`):**
   - Get current UTC time
   - Convert to ET
   - Invalidate window to request repaint

4. **On Paint Request (`WM_PAINT`):**
   - Begin painting
   - Fill background
   - Format time string
   - Draw time string
   - End painting

5. **On Tray Icon Interaction (`WM_TRAYICON`):**
   - Left-click: Toggle window visibility
   - Right-click: Display context menu

6. **On Menu Command (`WM_COMMAND`):**
   - Toggle Format: Switch formats, adjust window
   - Exit: Destroy window

7. **Termination (`WM_DESTROY`):**
   - Remove system tray icon
   - Destroy timer
   - Delete font object
   - Post quit message

## Conclusion

This architecture leverages Windows API functions to create an efficient, native transparent overlay clock. By separating concerns into distinct components, the architecture remains modular and maintainable.