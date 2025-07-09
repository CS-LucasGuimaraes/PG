# ===================================================================
# Prism Library | Library Target
#
# This CMakeLists.txt file defines the main shared library target,
# manages its dependencies, and sets up installation rules.
# ===================================================================

# --- Dependencies ---
include(FetchContent)
FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  # Pinned to a specific commit to fix build issues with modern compilers.
  GIT_TAG        2f86d13775d119edbb69af52e5f566fd65c6953b
)
FetchContent_MakeAvailable(yaml-cpp)

# --- Target Definition ---

# Collect all source files for the library.
file(GLOB_RECURSE PRISM_SOURCES CONFIGURE_DEPENDS "src/*.cpp")

# Define the library target as a shared library.
add_library(Prism SHARED ${PRISM_SOURCES})

# Automatically generate export headers for Windows/Linux/macOS.
include(GenerateExportHeader)
generate_export_header(Prism)

# --- Target Properties ---

# Define the public and private include directories.
target_include_directories(Prism
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Link the library against its dependencies.
target_link_libraries(Prism PUBLIC yaml-cpp)

# --- Installation ---
install(TARGETS Prism
    EXPORT PrismTargets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)
# Install the public headers.
install(DIRECTORY include/Prism/ DESTINATION include/Prism)