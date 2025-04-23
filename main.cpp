/*
 * EST Clock - A transparent Eastern Time clock for Windows
 * Copyright (c) 2025 EST Clock Contributors
 *
 * This software is released under the MIT License.
 * See the LICENSE file for details.
 */

#include <windows.h>
#include <tchar.h>
#include <strsafe.h> // For StringCchPrintf
#include <shellapi.h> // For Shell_NotifyIcon
#include <commctrl.h> // For NOTIFYICONDATA_V2_SIZE and NOTIFYICON_VERSION
#include "resource.h" // Include resource definitions (IDI_APPICON)

// Ensure we're using the correct version of the Windows Common Controls
#pragma comment(lib, "comctl32.lib")
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#define WINDOW_CLASS_NAME _T("ESTOverlayClockClass")
#define WINDOW_TITLE _T("EST Overlay Clock")
#define TIMER_ID 1 // Identifier for our timer
#define IDM_TOGGLE_FORMAT 101
#define IDM_EXIT 102
#define WM_TRAYICON (WM_USER + 1) // Custom message for tray icon

// Global variables
HWND g_hwnd = NULL;
SYSTEMTIME g_etTime; // Stores the current Eastern Time
HFONT g_hFont = NULL;
bool g_use12HourFormat = true; // Default to 12-hour format
NOTIFYICONDATA g_nid; // Notification icon data

// Forward declaration of the window procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Function to get Eastern Time Zone information
BOOL GetEasternTimeZoneInfo(TIME_ZONE_INFORMATION* pTzi) {
    // Standard Name: "Eastern Standard Time" (This is the key in the registry)
    // Display Name varies by language, but the Standard Name is consistent.
    DWORD dwResult = GetTimeZoneInformationForYear(2024, NULL, pTzi); // Year doesn't matter much here for standard info

    // Fallback or specific lookup if needed, but GetTimeZoneInformation should work if the TZ is known to Windows.
    // For robustness, you might iterate through registry keys under HKLM\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Time Zones
    // But GetTimeZoneInformation is the standard way.
    if (dwResult == TIME_ZONE_ID_INVALID) {
        // Handle error - Time Zone info not found
        // For simplicity, we'll assume it's found. A real app needs error handling.
        OutputDebugString(_T("Failed to get Time Zone Information.\n"));
        // Set some defaults maybe? Or fail. For now, let's try setting manually for EST/EDT
        // These values might need adjustment based on current DST rules if GetTimeZoneInformation fails.
        ZeroMemory(pTzi, sizeof(TIME_ZONE_INFORMATION));
        pTzi->Bias = 300; // UTC - 5 hours (EST)
        pTzi->StandardBias = 0; // No additional bias for standard time
        StringCchCopy(pTzi->StandardName, 32, L"Eastern Standard Time");
        // DST settings (example - adjust based on current rules if needed)
        pTzi->DaylightBias = -60; // Add 60 minutes for EDT (UTC - 4 hours)
        StringCchCopy(pTzi->DaylightName, 32, L"Eastern Daylight Time");
        // Example: DST starts second Sunday in March, 2:00 AM
        pTzi->StandardDate.wMonth = 11; // November
        pTzi->StandardDate.wDayOfWeek = 0; // Sunday
        pTzi->StandardDate.wDay = 1; // First Sunday
        pTzi->StandardDate.wHour = 2;
        // Example: DST ends first Sunday in November, 2:00 AM
        pTzi->DaylightDate.wMonth = 3; // March
        pTzi->DaylightDate.wDayOfWeek = 0; // Sunday
        pTzi->DaylightDate.wDay = 2; // Second Sunday
        pTzi->DaylightDate.wHour = 2;
        // return FALSE; // Indicate failure or use manual settings
        OutputDebugString(_T("Using manually configured EST/EDT settings as fallback.\n"));
        return TRUE; // Proceed with manual settings
    }
    return TRUE;
}

// Function to update the time
void UpdateTime(HWND hwnd) {
    SYSTEMTIME utcTime;
    TIME_ZONE_INFORMATION tzi;

    GetSystemTime(&utcTime); // Get current UTC time

    if (GetEasternTimeZoneInfo(&tzi)) {
        if (!SystemTimeToTzSpecificLocalTime(&tzi, &utcTime, &g_etTime)) {
            // Handle error - conversion failed
            DWORD dwError = GetLastError();
            TCHAR szErrorMsg[256];
            StringCchPrintf(szErrorMsg, 256, _T("SystemTimeToTzSpecificLocalTime failed with error %d\n"), dwError);
            OutputDebugString(szErrorMsg);
            // Maybe set a default or error state for g_etTime
        }
    } else {
        // Handle error - could not get TZ info
        // Maybe set a default or error state for g_etTime
    }

    // Invalidate the window to trigger a repaint
    InvalidateRect(hwnd, NULL, TRUE); // TRUE to erase background
}

// Helper function to calculate window width based on format
int GetWindowWidthForFormat(bool use12HourFormat) {
    return use12HourFormat ? 240 : 200; // Wider for 12-hour format
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc;
    MSG msg;

    // Register the window class
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = 0;
    wc.lpfnWndProc = WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPICON)); // Use embedded icon
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL; // Prevent system background painting
    wc.lpszMenuName = NULL;
    wc.lpszClassName = WINDOW_CLASS_NAME;
    wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPICON)); // Use embedded icon

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, _T("Window Registration Failed!"), _T("Error!"), MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Create the window
    // Position bottom-right for now, adjust as needed
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);
    int windowWidth = g_use12HourFormat ? 240 : 200; // Set width based on format
    int windowHeight = 50;
    int posX = screenWidth - windowWidth - 20; // 20px padding
    int posY = screenHeight - windowHeight - 50; // 50px padding from bottom (taskbar)

    g_hwnd = CreateWindowEx(
        WS_EX_LAYERED | WS_EX_TOPMOST | WS_EX_TOOLWINDOW, // Layered, Topmost, No Taskbar icon
        WINDOW_CLASS_NAME,
        WINDOW_TITLE,
        WS_POPUP, // Borderless popup window
        posX, posY, windowWidth, windowHeight,
        NULL, NULL, hInstance, NULL);

    if (g_hwnd == NULL) {
        MessageBox(NULL, _T("Window Creation Failed!"), _T("Error!"), MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Set layered window attributes for transparency (e.g., make black transparent)
    // SetLayeredWindowAttributes(g_hwnd, RGB(0, 0, 0), 0, LWA_COLORKEY);
    // Or set overall alpha (e.g., 200 out of 255 for slight transparency)
    SetLayeredWindowAttributes(g_hwnd, 0, 175, LWA_ALPHA); // Increased transparency (was 200)

    ShowWindow(g_hwnd, nCmdShow);
    UpdateWindow(g_hwnd);

    // Message loop
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE:
        {
            // Create the font
            g_hFont = CreateFont(
                32, 0, 0, 0, // Height, Width, Escapement, Orientation
                FW_BOLD,    // Weight
                FALSE, FALSE, FALSE, // Italic, Underline, Strikeout
                DEFAULT_CHARSET, OUT_OUTLINE_PRECIS, CLIP_DEFAULT_PRECIS,
                CLEARTYPE_QUALITY, // Quality
                VARIABLE_PITCH | FF_SWISS, // Pitch and Family
                _T("Arial") // Font name
            );

            // Set the timer to update every second
            SetTimer(hwnd, TIMER_ID, 1000, NULL);
            // Initial time update
            UpdateTime(hwnd);

            // Initialize Common Controls (required for modern Windows UI elements)
            INITCOMMONCONTROLSEX icex;
            icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
            icex.dwICC = ICC_WIN95_CLASSES;
            InitCommonControlsEx(&icex);

            // Create the tray icon immediately instead of using a timer
            // Initialize the tray icon
            ZeroMemory(&g_nid, sizeof(NOTIFYICONDATA));
            g_nid.cbSize = sizeof(NOTIFYICONDATA);
            g_nid.hWnd = hwnd;
            g_nid.uID = 1;
            g_nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
            g_nid.uCallbackMessage = WM_TRAYICON;

            // Try multiple approaches to load the icon
            HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);

            // Get system metrics for small icon size
            int iconX = GetSystemMetrics(SM_CXSMICON);
            int iconY = GetSystemMetrics(SM_CYSMICON);

            // Approach 1: Try to load the icon directly from the file
            g_nid.hIcon = (HICON)LoadImage(
                NULL,
                _T("windows-est-clock.ico"),
                IMAGE_ICON,
                iconX,
                iconY,
                LR_LOADFROMFILE | LR_DEFAULTCOLOR
            );

            // Approach 2: If that fails, try to load from resource with specific size
            if (g_nid.hIcon == NULL) {
                g_nid.hIcon = (HICON)LoadImage(
                    hInstance,
                    MAKEINTRESOURCE(IDI_APPICON),
                    IMAGE_ICON,
                    iconX,
                    iconY,
                    LR_DEFAULTCOLOR
                );
            }

            // Approach 3: If that fails, try LoadIcon
            if (g_nid.hIcon == NULL) {
                g_nid.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_APPICON));
            }

            // If all approaches fail, fall back to system icon
            if (g_nid.hIcon == NULL) {
                g_nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
                OutputDebugString(_T("Failed to load custom icon, using system icon"));
            }

            // Set tooltip
            StringCchCopy(g_nid.szTip, ARRAYSIZE(g_nid.szTip), _T("EST Clock"));

            // Add the icon
            BOOL result = Shell_NotifyIcon(NIM_ADD, &g_nid);

            // Set a timer to periodically refresh the icon
            SetTimer(hwnd, 4, 10000, NULL); // Timer ID 4, refresh every 10 seconds

            // Debug message
            OutputDebugString(result ? _T("Tray icon added successfully") : _T("Failed to add tray icon"));
        }
        break;

    case WM_TIMER:
        if (wParam == TIMER_ID) {
            UpdateTime(hwnd);
        }
        else if (wParam == 4) { // Icon refresh timer
            // Periodically refresh the icon to ensure it stays visible
            // Update both the icon and tooltip
            g_nid.uFlags = NIF_ICON | NIF_TIP;

            // Make sure we still have a valid icon
            if (g_nid.hIcon == NULL) {
                // Try to reload the icon
                HINSTANCE hInstance = (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE);
                int iconX = GetSystemMetrics(SM_CXSMICON);
                int iconY = GetSystemMetrics(SM_CYSMICON);

                // Try from file first
                g_nid.hIcon = (HICON)LoadImage(
                    NULL,
                    _T("windows-est-clock.ico"),
                    IMAGE_ICON,
                    iconX,
                    iconY,
                    LR_LOADFROMFILE | LR_DEFAULTCOLOR
                );

                // If that fails, try from resource
                if (g_nid.hIcon == NULL) {
                    g_nid.hIcon = (HICON)LoadImage(
                        hInstance,
                        MAKEINTRESOURCE(IDI_APPICON),
                        IMAGE_ICON,
                        iconX,
                        iconY,
                        LR_DEFAULTCOLOR
                    );
                }

                // If still NULL, use system icon
                if (g_nid.hIcon == NULL) {
                    g_nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
                }
            }

            // Update tooltip with current time
            TCHAR timeText[50];
            StringCchPrintf(timeText, 50, _T("EST Clock - %02d:%02d:%02d"),
                g_etTime.wHour, g_etTime.wMinute, g_etTime.wSecond);
            StringCchCopy(g_nid.szTip, ARRAYSIZE(g_nid.szTip), timeText);

            // Apply the updates
            Shell_NotifyIcon(NIM_MODIFY, &g_nid);
        }
        break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Get client area rectangle
            RECT rect;
            GetClientRect(hwnd, &rect);

            // Create a black brush (will be transparent due to LWA_ALPHA)
            HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0));
            // Fill the background to erase previous text
            FillRect(hdc, &rect, hBrush);
            // Clean up the brush
            DeleteObject(hBrush);

            // Set up DC for text
            SetBkMode(hdc, TRANSPARENT); // Make background transparent for text
            SetTextColor(hdc, RGB(255, 255, 255)); // White text
            if (g_hFont) {
                SelectObject(hdc, g_hFont);
            }

            // Format the time string based on the flag
            TCHAR timeText[50];
            if (g_use12HourFormat) {
                int hour12 = g_etTime.wHour % 12;
                if (hour12 == 0) hour12 = 12; // Handle midnight (0 -> 12) and noon (12 -> 12)
                LPCTSTR ampm = (g_etTime.wHour < 12) ? _T("AM") : _T("PM");
                StringCchPrintf(timeText, 50, _T("%d:%02d:%02d %s EST"),
                    hour12, g_etTime.wMinute, g_etTime.wSecond, ampm);
            } else {
                StringCchPrintf(timeText, 50, _T("%02d:%02d:%02d EST"),
                    g_etTime.wHour, g_etTime.wMinute, g_etTime.wSecond);
            }

            // Draw the text centered
            DrawText(hdc, timeText, -1, &rect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

            EndPaint(hwnd, &ps);
        }
        break;

     // Allow dragging the window
    case WM_NCHITTEST:
        {
            LRESULT hit = DefWindowProc(hwnd, msg, wParam, lParam);
            if (hit == HTCLIENT) {
                return HTCAPTION; // Treat client area as caption for dragging
            }
            return hit;
        }
        break;

    case WM_TRAYICON:
        {
            if (lParam == WM_RBUTTONUP || lParam == WM_CONTEXTMENU) {
                // Display context menu when right-clicking the tray icon
                POINT pt;
                GetCursorPos(&pt);

                // Create popup menu
                HMENU hMenu = CreatePopupMenu();
                if (hMenu) {
                    // Add menu items
                    AppendMenu(hMenu, MF_STRING, IDM_TOGGLE_FORMAT,
                        g_use12HourFormat ? _T("Switch to 24-hour") : _T("Switch to 12 AM/PM"));
                    AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
                    AppendMenu(hMenu, MF_STRING, IDM_EXIT, _T("Exit"));

                    // Required to make menu work with SetForegroundWindow
                    SetForegroundWindow(hwnd);

                    // Display the menu
                    TrackPopupMenu(hMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN,
                        pt.x, pt.y, 0, hwnd, NULL);

                    // MSDN recommends sending this message after TrackPopupMenu
                    PostMessage(hwnd, WM_NULL, 0, 0);

                    DestroyMenu(hMenu);
                }
            }
            else if (lParam == WM_LBUTTONUP || lParam == WM_LBUTTONDBLCLK) {
                // Show/hide the main window when left-clicking the tray icon
                if (IsWindowVisible(hwnd)) {
                    ShowWindow(hwnd, SW_HIDE);
                } else {
                    ShowWindow(hwnd, SW_SHOW);
                    SetForegroundWindow(hwnd);
                }
            }
        }
        break;

    case WM_CONTEXTMENU:
        {
            HMENU hMenu = CreatePopupMenu();
            if (hMenu) {
                // Add menu items
                AppendMenu(hMenu, MF_STRING, IDM_TOGGLE_FORMAT, g_use12HourFormat ? _T("Use 24-Hour Format") : _T("Use 12-Hour Format"));
                AppendMenu(hMenu, MF_SEPARATOR, 0, NULL);
                AppendMenu(hMenu, MF_STRING, IDM_EXIT, _T("Exit"));

                // Display the menu at the cursor position
                POINT pt;
                GetCursorPos(&pt);
                TrackPopupMenu(hMenu, TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, NULL);

                DestroyMenu(hMenu);
            }
        }
        break;

    case WM_COMMAND:
        switch (LOWORD(wParam)) {
            case IDM_TOGGLE_FORMAT:
                {
                    g_use12HourFormat = !g_use12HourFormat;

                    // Adjust window width based on format
                    RECT rect;
                    GetWindowRect(hwnd, &rect);
                    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
                    int newWidth = GetWindowWidthForFormat(g_use12HourFormat);

                    // Reposition window to maintain right edge alignment
                    int newX = screenWidth - newWidth - 20; // 20px padding

                    // Resize and reposition the window
                    SetWindowPos(hwnd, NULL, newX, rect.top, newWidth, rect.bottom - rect.top,
                                 SWP_NOZORDER | SWP_NOACTIVATE);

                    InvalidateRect(hwnd, NULL, TRUE); // Force repaint with new format
                }
                break;
            case IDM_EXIT:
                DestroyWindow(hwnd);
                break;
        }
        break;

    case WM_DESTROY:
        // Remove the tray icon
        Shell_NotifyIcon(NIM_DELETE, &g_nid);

        // Clean up timers
        KillTimer(hwnd, TIMER_ID);
        KillTimer(hwnd, 4);

        if (g_hFont) {
            DeleteObject(g_hFont); // Clean up the font
        }
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
