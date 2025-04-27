/*
 * EST Clock - A transparent Eastern Time clock for Windows
 * Copyright (c) 2025 EST Clock Contributors
 *
 * This software is released under the MIT License.
 * See the LICENSE file for details.
 */

#pragma once

#include <windows.h>

// Function to get Eastern Time Zone information
BOOL GetEasternTimeZoneInfo(TIME_ZONE_INFORMATION* pTzi);

// Function to update the time
void UpdateTime(HWND hwnd);
