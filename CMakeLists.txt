# ===================================================================
# Prism Library | Root CMakeLists.txt
#
# This is the main project file that orchestrates the entire build.
# It sets global settings and includes the sub-projects (library, app, tests).
# ===================================================================
cmake_minimum_required(VERSION 3.22)
project(Prism VERSION 1.0.0 LANGUAGES CXX)

# Set the policy for FetchContent to use modern, safer behavior.
cmake_policy(SET CMP0135 NEW)

# --- Global Project Settings ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Build Options ---
# Add an option to control whether the demo application is built.
# Defaults to ON for typical development, but can be turned off.
option(PRISM_BUILD_DEMO "Build the Prism demo application" ON)

# Set a common output directory for all executables.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# --- Sub-projects ---
add_subdirectory(src) # The Prism library
add_subdirectory(demo) # The demo application

# --- Testing ---
enable_testing()
if(BUILD_TESTING)
    # Fetch GoogleTest only when tests are enabled.
    include(FetchContent)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/v1.14.0.zip
    )
    FetchContent_MakeAvailable(googletest)

    add_subdirectory(tests)
endif()