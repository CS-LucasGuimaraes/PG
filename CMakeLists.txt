# ===================================================================
# Prism Library | Root CMakeLists.txt
#
# This is the main project file that orchestrates the entire build.
# It sets global settings and includes the sub-projects.
# ===================================================================
cmake_minimum_required(VERSION 3.22)
project(Prism VERSION 0.1.0 LANGUAGES CXX)

# Set the policy for FetchContent to use modern, safer behavior.
cmake_policy(SET CMP0135 NEW)

# --- Global Project Settings ---
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --- Build Options ---
# Add an option to control whether the demo application is built.
# Defaults to ON for typical development, but can be turned off.
set(BUILD_SHARED_LIBS ON CACHE BOOL "Build shared libraries" FORCE)
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
    set(INSTALL_GTEST OFF CACHE BOOL "Disable GTest installation" FORCE)
    set(BUILD_GTEST_INSTALL OFF CACHE BOOL "Disable GTest installation" FORCE)
    FetchContent_MakeAvailable(googletest)


    add_subdirectory(tests)
endif()