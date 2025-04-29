# EST Clock Tests

This directory contains tests for the EST Clock application, focusing on time zone conversion and ensuring the application correctly displays Eastern Time regardless of the user's system settings.

## Test Categories

1. **Basic Time Conversion Tests** (`time_conversion_tests.cpp`)
   - Tests basic time conversion functionality
   - Verifies that Eastern Time Zone information can be retrieved
   - Checks that the time update function works correctly

2. **Time Zone Conversion Tests** (`time_zone_conversion_tests.cpp`)
   - Tests conversion from various time zones to Eastern Time
   - Includes tests for international date line crossing, DST transitions, and leap years
   - Uses mock implementations to avoid requiring system changes

3. **System Time Zone Tests** (`system_time_zone_tests.cpp`)
   - **Requires administrator privileges**
   - Tests that the application maintains Eastern Time when the system time zone changes
   - Tests handling of DST transitions with actual system time changes
   - Tests behavior during rapid time changes and across date boundaries

## Building and Running Tests

### Using the Test Runner Script

The easiest way to build and run all tests is to use the provided test runner script:

#### PowerShell (Recommended)

```powershell
.\run_tests_with_resync.ps1
```

#### Batch File

```cmd
.\run_tests_with_resync.bat
```

These scripts will:
1. Configure the tests with CMake
2. Build all test executables
3. Run all tests using CTest
4. Resynchronize the system time after tests to prevent any lingering time changes

### Manual Build and Test

If you prefer to build and run tests manually:

#### Basic Build

```powershell
mkdir -p build && cd build
cmake ..
cmake --build . --config Debug
```

#### Running Tests Individually

##### Basic Tests (No Admin Required)

```powershell
.\Debug\time_conversion_tests.exe
.\Debug\time_zone_conversion_tests.exe
```

##### System Time Zone Tests (Admin Required)

These tests require administrator privileges because they modify system time and time zone settings:

1. **Run PowerShell or Command Prompt as Administrator**:
   - Search for "PowerShell" or "Command Prompt" in the Start menu
   - Right-click and select "Run as administrator"

2. **Navigate to the test directory**:
   ```powershell
   cd path\to\EST-clock\tests\build\Debug
   ```

3. **Run the tests**:
   ```powershell
   .\system_time_zone_tests.exe
   ```

If you run these tests without administrator privileges, they will automatically skip the tests that require system changes and display a warning message.

### Running Tests with CTest

To run all tests using CTest:

```powershell
cd build
ctest -C Debug -V
```

The `-V` flag provides verbose output showing the details of each test.

## Test Implementation Details

### Privilege Management

The system time zone tests use the `PrivilegeHelper` class to:
1. Check if the tests are running with administrator privileges
2. Enable the necessary privileges for time zone and system time manipulation
3. Skip tests that require privileges when they're not available

### Time Zone Simulation

For tests that don't require actual system changes, we use mock implementations that simulate time zone conversions without modifying system settings.

### System Time Zone Tests

These tests verify that the EST Clock application:
1. Correctly displays Eastern Time regardless of the system time zone
2. Handles DST transitions properly
3. Works correctly across the international date line
4. Handles leap years correctly
5. Maintains Eastern Time when the system time zone changes while the application is running
6. Handles rapid time changes correctly

## Adding New Tests

When adding new tests:
1. For basic functionality that doesn't require system changes, add to `time_conversion_tests.cpp`
2. For time zone conversion tests that use mocks, add to `time_zone_conversion_tests.cpp`
3. For tests that require system changes, add to `system_time_zone_tests.cpp`
4. Make sure to handle the case where administrator privileges are not available

## Troubleshooting

### Common Issues

#### Tests Failing Due to Time Zone Issues

If you see failures in the `SystemTimeZoneTest.MaintainsEasternTimeAcrossTimeZones` or `SystemTimeZoneTest.HandlesRapidTimeChanges` tests, it may be due to:

1. **Day/Hour Mismatches**: The tests expect specific day and hour values. If you see errors like:
   ```
   Expected equality of these values:
     expectedET.wDay
       Which is: 2
     g_etTime.wDay
       Which is: 1
   ```
   This indicates a mismatch in the expected vs. actual time conversion.

2. **Time Resynchronization Failures**: After tests that modify the system time, the time should be resynchronized. If you see:
   ```
   Failed to resynchronize time. Error code: -2147023834
   ```
   This may indicate that the Windows Time service is not running or you don't have sufficient privileges.

#### Solutions

1. **Run as Administrator**: Ensure you're running the tests with administrator privileges.
2. **Use the Test Runner Script**: The `run_tests_with_resync.ps1` script includes proper time resynchronization.
3. **Manual Time Resync**: If tests fail and leave your system time incorrect, run:
   ```powershell
   w32tm /resync
   ```
   in an administrator PowerShell.

### Compilation Errors

If you encounter compilation errors related to `STARTUPINFO` vs `STARTUPINFOA`, ensure you're using the correct type for the Windows API functions you're calling. The `CreateProcessA` function requires `STARTUPINFOA` while `CreateProcessW` requires `STARTUPINFO`.
