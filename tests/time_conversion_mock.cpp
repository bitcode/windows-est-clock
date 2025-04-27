/*
 * EST Clock - A transparent Eastern Time clock for Windows
 * Copyright (c) 2025 EST Clock Contributors
 *
 * This software is released under the MIT License.
 * See the LICENSE file for details.
 */

#include <windows.h>
#include "time_conversion.h"
#include "globals.h"

// Forward declarations
BOOL MockSystemTimeToTzSpecificLocalTime(
    CONST TIME_ZONE_INFORMATION* lpTimeZoneInformation,
    CONST SYSTEMTIME* lpUniversalTime,
    LPSYSTEMTIME lpLocalTime);

// Mock time zone information for testing
static TIME_ZONE_INFORMATION g_mockETTimeZone = {
    300,                          // Bias (UTC-5 for EST)
    L"Eastern Standard Time",     // StandardName
    {0},                          // StandardDate
    0,                            // StandardBias
    L"Eastern Daylight Time",     // DaylightName
    {0},                          // DaylightDate
    -60                           // DaylightBias (UTC-4 for EDT)
};

// Mock current time zone for testing
static TIME_ZONE_INFORMATION g_mockCurrentTimeZone = {
    300,                          // Bias (UTC-5 for EST)
    L"Eastern Standard Time",     // StandardName
    {0},                          // StandardDate
    0,                            // StandardBias
    L"Eastern Daylight Time",     // DaylightName
    {0},                          // DaylightDate
    -60                           // DaylightBias (UTC-4 for EDT)
};

// Mock system time for testing
static SYSTEMTIME g_mockSystemTime = {2024, 1, 1, 1, 12, 0, 0, 0}; // Default to January 1, 2024, 12:00:00

// Mock function to set system time for testing
BOOL SetMockSystemTime(const SYSTEMTIME& time) {
    g_mockSystemTime = time;
    return TRUE;
}

// Mock function to get system time for testing
void GetMockSystemTime(SYSTEMTIME* pTime) {
    *pTime = g_mockSystemTime;
}

// Mock function to set time zone information
BOOL MockSetTimeZoneInformation(CONST TIME_ZONE_INFORMATION* lpTimeZoneInformation) {
    g_mockCurrentTimeZone = *lpTimeZoneInformation;
    return TRUE;
}

// Mock function to get time zone information
DWORD MockGetTimeZoneInformation(LPTIME_ZONE_INFORMATION lpTimeZoneInformation) {
    *lpTimeZoneInformation = g_mockCurrentTimeZone;
    return TIME_ZONE_ID_STANDARD;
}

// Mock function to get Eastern Time Zone information
BOOL GetEasternTimeZoneInfo(TIME_ZONE_INFORMATION* pTzi) {
    *pTzi = g_mockETTimeZone;
    return TRUE;
}

// Mock function to update the time
void UpdateTime(HWND hwnd) {
    SYSTEMTIME utcTime;
    TIME_ZONE_INFORMATION tzi;

    // Get current mock UTC time
    utcTime = g_mockSystemTime;

    if (GetEasternTimeZoneInfo(&tzi)) {
        MockSystemTimeToTzSpecificLocalTime(&tzi, &utcTime, &g_etTime);
    }
}

// Mock implementation of SystemTimeToTzSpecificLocalTime
BOOL MockSystemTimeToTzSpecificLocalTime(
    CONST TIME_ZONE_INFORMATION* lpTimeZoneInformation,
    CONST SYSTEMTIME* lpUniversalTime,
    LPSYSTEMTIME lpLocalTime) {

    // Simple implementation for testing
    *lpLocalTime = *lpUniversalTime;

    // Apply the time zone bias (in minutes)
    LONG totalMinutes = lpUniversalTime->wHour * 60 + lpUniversalTime->wMinute;
    totalMinutes -= lpTimeZoneInformation->Bias; // Subtract bias (negative for east of UTC)

    // Handle day changes
    int dayOffset = 0;
    while (totalMinutes < 0) {
        totalMinutes += 24 * 60;
        dayOffset--;
    }
    while (totalMinutes >= 24 * 60) {
        totalMinutes -= 24 * 60;
        dayOffset++;
    }

    // Set the new time
    lpLocalTime->wHour = (WORD)(totalMinutes / 60);
    lpLocalTime->wMinute = (WORD)(totalMinutes % 60);

    // Adjust the date if needed
    if (dayOffset != 0) {
        // This is a simplified date adjustment that doesn't handle month/year boundaries
        lpLocalTime->wDay += (WORD)dayOffset;
    }

    return TRUE;
}
