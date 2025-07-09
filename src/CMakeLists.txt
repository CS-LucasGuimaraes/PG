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

# --- Build Options ---

# Option to build the core math and data structures (mandatory)
option(PRISM_BUILD_CORE "Build the Core module (math, etc.)" ON)

# Optional modules
option(PRISM_BUILD_OBJECTS "Build the Objects module (Sphere, Plane, Mesh, etc.)" ON)
option(PRISM_BUILD_IO "Build the I/O module (SceneParser, ObjReader)" ON)
option(PRISM_BUILD_SCENE "Build the Scene module (Scene, Camera)" ON)

# The CORE module is essential for all other parts of the library.
if(NOT PRISM_BUILD_CORE)
    message(FATAL_ERROR "PRISM_BUILD_CORE is a required module and cannot be disabled.")
endif()


# --- Build Source File Collection ---

# Start with an empty list for our library's source files.
set(PRISM_SOURCES "")

# Always add the core source files.
if(PRISM_BUILD_CORE)
    file(GLOB CORE_SOURCES CONFIGURE_DEPENDS "src/core/*.cpp")
    list(APPEND PRISM_SOURCES ${CORE_SOURCES})
endif()

# Conditionally add sources for the other modules.
if(PRISM_BUILD_OBJECTS)
    file(GLOB GEOMETRY_SOURCES CONFIGURE_DEPENDS "src/objects/*.cpp")
    list(APPEND PRISM_SOURCES ${GEOMETRY_SOURCES})
endif()

if(PRISM_BUILD_IO)
    file(GLOB IO_SOURCES CONFIGURE_DEPENDS "src/io/*.cpp")
    list(APPEND PRISM_SOURCES ${IO_SOURCES})
endif()

if(PRISM_BUILD_SCENE)
    file(GLOB SCENE_SOURCES CONFIGURE_DEPENDS "src/scene/*.cpp")
    list(APPEND PRISM_SOURCES ${SCENE_SOURCES})
endif()

# Add any remaining top-level source files (e.g., init.cpp)
file(GLOB OTHER_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
list(APPEND PRISM_SOURCES ${OTHER_SOURCES})


# --- Target Definition ---

# Define the library target using the conditionally populated source list.
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
# The yaml-cpp dependency is only needed if the IO module is built.
if(PRISM_BUILD_IO)
    target_link_libraries(Prism PUBLIC yaml-cpp)
endif()


# --- Installation ---

install(TARGETS Prism
    EXPORT PrismTargets
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# Install the public headers, preserving the subdirectory structure.
install(DIRECTORY
    include/Prism/core
    include/Prism/objects
    include/Prism/io
    include/Prism/scene
    DESTINATION include/Prism
)

# Install any top-level header files like Prism.hpp
install(FILES
    include/Prism.hpp
    DESTINATION include
)