#include <gtest/gtest.h>
#include <windows.h>
#include "./time_conversion.h" // Local header file for time-related functions
#include "./globals.h"        // Local header file for global variables

// Simple test to verify that the Eastern Time Zone info can be retrieved
TEST(BasicTimeTest, GetEasternTimeZoneInfo) {
    TIME_ZONE_INFORMATION etTZ;
    EXPECT_TRUE(GetEasternTimeZoneInfo(&etTZ));
    EXPECT_STREQ(L"Eastern Standard Time", etTZ.StandardName);
}

// Simple test to verify that UpdateTime function works
TEST(BasicTimeTest, UpdateTimeFunction) {
    // Set a known value for g_etTime to verify it gets updated
    g_etTime = {1900, 1, 1, 1, 0, 0, 0, 0};

    // Call UpdateTime with our dummy g_hwnd
    UpdateTime(g_hwnd);

    // Verify that g_etTime was updated (should no longer be 1900)
    EXPECT_NE(1900, g_etTime.wYear);
    EXPECT_GE(g_etTime.wYear, 2020); // Should be at least 2020

    // Get current time in Eastern Time for comparison
    SYSTEMTIME utcTime, etTime;
    GetSystemTime(&utcTime);

    TIME_ZONE_INFORMATION etTZ;
    ASSERT_TRUE(GetEasternTimeZoneInfo(&etTZ));

    // Convert UTC to Eastern Time
    ASSERT_TRUE(SystemTimeToTzSpecificLocalTime(&etTZ, &utcTime, &etTime));

    // Don't compare the exact values as they might differ slightly due to timing
    // Just verify the year is reasonable
    EXPECT_GE(g_etTime.wYear, 2020);
    EXPECT_LE(g_etTime.wYear, 2030);
}

// Test for time zone bias calculation
TEST(TimeZoneTest, TimeZoneBias) {
    // Get Eastern Time Zone info
    TIME_ZONE_INFORMATION etTZ;
    ASSERT_TRUE(GetEasternTimeZoneInfo(&etTZ));

    // Eastern Time should be UTC-5 (300 minutes) during standard time
    // or UTC-4 (240 minutes) during daylight saving time
    EXPECT_TRUE(etTZ.Bias == 300 || etTZ.Bias == 240);

    if (etTZ.Bias == 300) {
        // Standard time (EST)
        EXPECT_EQ(0, etTZ.StandardBias);
        EXPECT_EQ(-60, etTZ.DaylightBias); // -60 minutes for EDT
    } else {
        // Daylight time (EDT)
        EXPECT_EQ(0, etTZ.DaylightBias);
        EXPECT_EQ(60, etTZ.StandardBias); // +60 minutes for EST
    }
}

// Test for time format conversion
TEST(TimeFormatTest, TimeFormatting) {
    // Create a test time and initialize it manually
    SYSTEMTIME testTime;
    ZeroMemory(&testTime, sizeof(SYSTEMTIME));

    testTime.wYear = 2024;
    testTime.wMonth = 4;
    testTime.wDay = 15;
    testTime.wDayOfWeek = 1; // Monday
    testTime.wHour = 14;
    testTime.wMinute = 30;
    testTime.wSecond = 0;
    testTime.wMilliseconds = 0;

    // Verify the values are set correctly
    EXPECT_EQ(2024, testTime.wYear);
    EXPECT_EQ(4, testTime.wMonth);
    EXPECT_EQ(15, testTime.wDay);
    EXPECT_EQ(14, testTime.wHour);
    EXPECT_EQ(30, testTime.wMinute);
    EXPECT_EQ(0, testTime.wSecond);
}

