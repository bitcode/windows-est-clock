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

# Add tests to CTest
add_test(NAME TimeConversionTests COMMAND time_conversion_tests)
add_test(NAME TimeZoneConversionTests COMMAND time_zone_conversion_tests)

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


