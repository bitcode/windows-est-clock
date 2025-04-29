/*
 * EST Clock - A transparent Eastern Time clock for Windows
 * Copyright (c) 2025 EST Clock Contributors
 *
 * This software is released under the MIT License.
 * See the LICENSE file for details.
 */

#pragma once

#include <windows.h>
#include <string>
#include <iostream>

// Class to manage Windows privileges for testing
class PrivilegeHelper {
public:
    // Enable a specific privilege for the current process
    static bool EnablePrivilege(const wchar_t* privilegeName) {
        HANDLE hToken;
        TOKEN_PRIVILEGES tkp;
        BOOL result = FALSE;

        // Try to open the process token with appropriate access rights
        if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
            std::cerr << "Failed to open process token. Error: " << GetLastError() << std::endl;
            return false;
        }

        // Look up the LUID for the privilege
        if (!LookupPrivilegeValue(NULL, privilegeName, &tkp.Privileges[0].Luid)) {
            std::cerr << "Failed to lookup privilege value. Error: " << GetLastError() << std::endl;
            CloseHandle(hToken);
            return false;
        }

        tkp.PrivilegeCount = 1;
        tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

        // Enable the privilege
        result = AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, NULL, 0);
        DWORD lastError = GetLastError();
        
        CloseHandle(hToken);

        if (!result || lastError != ERROR_SUCCESS) {
            std::cerr << "Failed to adjust token privileges. Error: " << lastError << std::endl;
            return false;
        }

        return true;
    }

    // Check if the current process has admin privileges
    static bool HasAdminPrivileges() {
        BOOL isAdmin = FALSE;
        SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
        PSID AdministratorsGroup;
        
        // Create a SID for the Administrators group
        if (!AllocateAndInitializeSid(
            &NtAuthority,
            2,
            SECURITY_BUILTIN_DOMAIN_RID,
            DOMAIN_ALIAS_RID_ADMINS,
            0, 0, 0, 0, 0, 0,
            &AdministratorsGroup)) {
            return false;
        }
        
        // Check if the current process token is a member of the Administrators group
        if (!CheckTokenMembership(NULL, AdministratorsGroup, &isAdmin)) {
            isAdmin = FALSE;
        }
        
        FreeSid(AdministratorsGroup);
        return isAdmin != FALSE;
    }

    // Enable privileges needed for time zone and system time manipulation
    static bool EnableTimeManipulationPrivileges() {
        bool timeZonePriv = EnablePrivilege(SE_TIME_ZONE_NAME);
        bool systemTimePriv = EnablePrivilege(SE_SYSTEMTIME_NAME);
        
        if (!timeZonePriv) {
            std::cerr << "Warning: Failed to enable time zone manipulation privilege." << std::endl;
        }
        
        if (!systemTimePriv) {
            std::cerr << "Warning: Failed to enable system time manipulation privilege." << std::endl;
        }
        
        return timeZonePriv && systemTimePriv;
    }
};
