cmake_minimum_required(VERSION 3.10)
project(ESTClockTests)

# Enable testing
enable_testing()

# Download and build GoogleTest
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Add test executable for basic time conversion tests
add_executable(time_conversion_tests
    time_conversion_tests.cpp
    globals.cpp
    time_conversion_mock.cpp
    # Don't include main.cpp as it will conflict with our mocks
    # ../main.cpp
)

# Add test executable for time zone conversion tests
add_executable(time_zone_conversion_tests
    time_zone_conversion_tests.cpp
    globals.cpp
    time_conversion_mock.cpp
)

# Add test executable for system time zone tests
add_executable(system_time_zone_tests
    system_time_zone_tests.cpp
    globals.cpp
    time_conversion_mock.cpp
)

# Set include directories for time_conversion_tests
target_include_directories(time_conversion_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/..
    ${CMAKE_SOURCE_DIR}
    ${gtest_SOURCE_DIR}/include
)

# Add header files directory if they're in a separate folder
target_include_directories(time_conversion_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/../include
)

# Set include directories for time_zone_conversion_tests
target_include_directories(time_zone_conversion_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/..
    ${CMAKE_SOURCE_DIR}
    ${gtest_SOURCE_DIR}/include
)

# Add header files directory for time_zone_conversion_tests
target_include_directories(time_zone_conversion_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/../include
)

# Set include directories for system_time_zone_tests
target_include_directories(system_time_zone_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/..
    ${CMAKE_SOURCE_DIR}
    ${gtest_SOURCE_DIR}/include
)

# Add header files directory for system_time_zone_tests
target_include_directories(system_time_zone_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/../include
)

# Link against GTest and required Windows libraries for time_conversion_tests
target_link_libraries(time_conversion_tests PRIVATE
    GTest::gtest
    GTest::gtest_main
    gdi32
    shell32
    comctl32
)

# Link against GTest and required Windows libraries for time_zone_conversion_tests
target_link_libraries(time_zone_conversion_tests PRIVATE
    GTest::gtest
    GTest::gtest_main
    gdi32
    shell32
    comctl32
)

# Link against GTest and required Windows libraries for system_time_zone_tests
target_link_libraries(system_time_zone_tests PRIVATE
    GTest::gtest
    GTest::gtest_main
    gdi32
    shell32
    comctl32
    advapi32  # Required for privilege management
)

# Add tests to CTest
add_test(NAME TimeConversionTests COMMAND time_conversion_tests)
add_test(NAME TimeZoneConversionTests COMMAND time_zone_conversion_tests)
add_test(NAME SystemTimeZoneTests COMMAND system_time_zone_tests)

# Set C++ standard for time_conversion_tests
set_target_properties(time_conversion_tests PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Set C++ standard for time_zone_conversion_tests
set_target_properties(time_zone_conversion_tests PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Set C++ standard for system_time_zone_tests
set_target_properties(system_time_zone_tests PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Add Windows-specific definitions for time_conversion_tests
target_compile_definitions(time_conversion_tests PRIVATE
    UNICODE
    _UNICODE
    WIN32
    _WINDOWS
)

# Add Windows-specific definitions for time_zone_conversion_tests
target_compile_definitions(time_zone_conversion_tests PRIVATE
    UNICODE
    _UNICODE
    WIN32
    _WINDOWS
)

# Add Windows-specific definitions for system_time_zone_tests
target_compile_definitions(system_time_zone_tests PRIVATE
    UNICODE
    _UNICODE
    WIN32
    _WINDOWS
)

# Create a script to resynchronize time after tests
file(WRITE ${CMAKE_BINARY_DIR}/resync_time.bat
    "@echo off\n"
    "echo Resynchronizing system time after tests...\n"
    "w32tm /resync\n"
    "if %ERRORLEVEL% neq 0 (\n"
    "    echo Failed to resynchronize time. Error code: %ERRORLEVEL%\n"
    ") else (\n"
    "    echo Time resynchronization completed successfully.\n"
    ")\n"
    "exit /b 0\n"
)

# Add a custom target to run the time resynchronization script
add_custom_target(resync_time
    COMMAND ${CMAKE_BINARY_DIR}/resync_time.bat
    COMMENT "Resynchronizing system time after tests"
    VERBATIM
)

# Add a post-test command to run the time resynchronization
add_custom_command(
    TARGET system_time_zone_tests
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E echo "Adding post-test time resynchronization hook"
)

# Set up a post-test hook for CTest
set_property(TEST SystemTimeZoneTests
    PROPERTY FIXTURES_CLEANUP resync_time_fixture
)

# Define the fixture setup
add_test(NAME ResyncTimeAfterTests
    COMMAND ${CMAKE_BINARY_DIR}/resync_time.bat
)
set_property(TEST ResyncTimeAfterTests
    PROPERTY FIXTURES_REQUIRED resync_time_fixture
)

# Add a custom command to run after all tests
add_custom_command(
    OUTPUT time_resync_sentinel
    COMMAND ${CMAKE_BINARY_DIR}/resync_time.bat
    COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_BINARY_DIR}/time_resync_sentinel
    COMMENT "Final time resynchronization after all tests"
    VERBATIM
)

# Add a custom target that depends on the sentinel
add_custom_target(final_time_resync ALL
    DEPENDS time_resync_sentinel
    COMMENT "Ensuring time resynchronization runs after all tests"
)

# Make sure the final resync runs after the tests
add_dependencies(final_time_resync system_time_zone_tests)
