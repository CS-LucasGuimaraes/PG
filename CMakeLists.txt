cmake_minimum_required(VERSION 3.14.0)
cmake_policy(SET CMP0135 NEW) # Policy to modern FetchContent behavior

project(PG_Project VERSION 0.0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
add_subdirectory(libs)
add_subdirectory(src)

if(BUILD_TESTING)
  ### include GoogleTest via FetchContent
  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/v1.14.0.zip
  )

  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)
  ###
  add_subdirectory(tests)
endif()

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(DIRECTORY data DESTINATION bin)