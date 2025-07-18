# ===================================================================
# Prism Library | Library Target
#
# This CMakeLists.txt file defines the main shared library target,
# manages its dependencies, and sets up installation rules.
# ===================================================================

message(STATUS "Configuring Prism Library (v${PROJECT_VERSION}).") 

# --- Dependencies ---

include(FetchContent)
FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  # Pinned to a specific commit to fix build issues with modern compilers.
  GIT_TAG        2f86d13775d119edbb69af52e5f566fd65c6953b
)
message(STATUS "Configuring yaml-cpp dependency: building SHARED library, tests/tools DISABLED.")

set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "Disable building yaml-cpp contrib tools" FORCE)
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "Disable building yaml-cpp parse tools" FORCE)
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "Disable building yaml-cpp tests" FORCE)
set(YAML_CPP_FORMAT_SOURCE OFF CACHE BOOL "Disable yaml-cpp source formatting" FORCE)
set(YAML_CPP_BUILD_SHARED_LIBS ON CACHE BOOL "Build yaml-cpp as shared library" FORCE) # Garanta que SHARED_LIBS também está com FORCE
set(YAML_CPP_INSTALL ON CACHE BOOL "Install yaml-cpp" FORCE)

FetchContent_MakeAvailable(yaml-cpp)


# --- Build Options ---

# Option to build the core math and data structures (mandatory)
option(PRISM_BUILD_CORE "Build the Core module (math, etc.)" ON)

# Optional modules
option(PRISM_BUILD_OBJECTS "Build the Objects module (Sphere, Plane, Mesh, etc.)" ON)
option(PRISM_BUILD_SCENE "Build the Scene module (Scene, Camera)" ON)

# --- Dependecy Management ---

# The CORE module is essential for all other parts of the library.
if(NOT PRISM_BUILD_CORE)
    message(FATAL_ERROR "PRISM_BUILD_CORE is a required module and cannot be disabled.")
endif()

# The OBJECTS module depends on the CORE module.
if(PRISM_BUILD_OBJECTS AND NOT PRISM_BUILD_CORE)
    message(FATAL_ERROR "PRISM_BUILD_OBJECTS requires PRISM_BUILD_CORE to be enabled. Please enable PRISM_BUILD_CORE.")
endif()

# The SCENE module depends on the OBJECTS module.
if(PRISM_BUILD_SCENE AND NOT PRISM_BUILD_OBJECTS)
    message(FATAL_ERROR "PRISM_BUILD_SCENE requires PRISM_BUILD_OBJECTS to be enabled. Please enable PRISM_BUILD_OBJECTS.")
endif()


# --- Build Source File Collection ---

# Start with an empty list for our library's source files.
set(PRISM_SOURCES "")

# Always add the core source files.
if(PRISM_BUILD_CORE)
    message(STATUS "Prism Library: Core module ENABLED.") 
    file(GLOB CORE_SOURCES CONFIGURE_DEPENDS "src/core/*.cpp")
    list(APPEND PRISM_SOURCES ${CORE_SOURCES})
endif()

# Conditionally add sources for the other modules.
if(PRISM_BUILD_OBJECTS)
    message(STATUS "Prism Library: Objects module ENABLED.")
    file(GLOB GEOMETRY_SOURCES CONFIGURE_DEPENDS "src/objects/*.cpp")
    list(APPEND PRISM_SOURCES ${GEOMETRY_SOURCES})
else()
    message(STATUS "Prism Library: Objects module DISABLED.")
endif()

if(PRISM_BUILD_SCENE)
    message(STATUS "Prism Library: Scene module ENABLED.")
    file(GLOB SCENE_SOURCES CONFIGURE_DEPENDS "src/scene/*.cpp")
    list(APPEND PRISM_SOURCES ${SCENE_SOURCES})
else()
    message(STATUS "Prism Library: Scene module DISABLED.")
endif()

file(GLOB VULKAN_SOURCES CONFIGURE_DEPENDS "src/vulkan/*.cpp")
list(APPEND PRISM_SOURCES ${VULKAN_SOURCES})

# Add any remaining top-level source files (e.g., init.cpp)
file(GLOB OTHER_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
list(APPEND PRISM_SOURCES ${OTHER_SOURCES})


# --- Target Definition ---

# Define the library target using the conditionally populated source list.
add_library(Prism SHARED ${PRISM_SOURCES})

add_dependencies(Prism CompileShaders)

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

# 
set_target_properties(Prism PROPERTIES
    # Define the library's version.
    VERSION "${PROJECT_VERSION}"
    # Define the SOVERSION, which is used for shared libraries.
    # This should match the major version of the library.
    SOVERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
    # Identify debug and release builds.
    DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}"
)

# Define the RPATH settings for the library.
set_target_properties(Prism PROPERTIES
    INSTALL_RPATH "$ORIGIN/."
)

# Link the library against its dependencies.
# The yaml-cpp dependency is only needed if the IO module is built.
if(PRISM_BUILD_OBJECTS)
    target_link_libraries(Prism PRIVATE 
        yaml-cpp::yaml-cpp
        Vulkan::Vulkan
    )
endif()
# Set Submodule flags for the library.
# This ensures that the library is built with the correct visibility settings.
if (PRISM_BUILD_CORE)
    target_compile_definitions(Prism PUBLIC PRISM_BUILD_CORE)
endif()

if (PRISM_BUILD_OBJECTS)
    target_compile_definitions(Prism PUBLIC PRISM_BUILD_OBJECTS)
endif()

if (PRISM_BUILD_SCENE)
    target_compile_definitions(Prism PUBLIC PRISM_BUILD_SCENE)
endif()

target_compile_definitions(Prism PRIVATE VULKAN_HPP_DISPATCH_LOADER_DYNAMIC_STORAGE)
target_compile_definitions(Prism PRIVATE VULKAN_HPP_DISPATCH_LOADER_DYNAMIC)

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
    include/Prism/scene
    DESTINATION include/Prism
)

# Install any top-level header files like Prism.hpp
install(FILES
    include/Prism.hpp
    DESTINATION include
)

# Install yaml-cpp headers and library if the OBJECTS module is built.
if(PRISM_BUILD_OBJECTS)

    install(DIRECTORY ${yaml-cpp_SOURCE_DIR}/include/yaml-cpp
        DESTINATION include
    )

    if(YAML_BUILD_SHARED_LIBS)
        install(TARGETS yaml-cpp
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib
        )
    endif()
endif()


# ===================================================================
# Shader Compilation Configuration
#
# This section configures the shader compilation process using glslc.
# It finds the glslc compiler, sets up the output directory,
# and creates custom commands to compile all shader files found in the
# 'src/shaders' directory.
# ===================================================================
message(STATUS "Configuring shader compilation...")

# 1. Find the glslc compiler.
# This is the compiler for GLSL shaders, part of the Vulkan SDK.
find_program(GLSLC_COMPILER glslc)
if(NOT GLSLC_COMPILER)
    message(FATAL_ERROR "glslc compiler not found! Please ensure the Vulkan SDK is installed and in your PATH.")
endif()
message(STATUS "Found glslc compiler: ${GLSLC_COMPILER}")

# 2. Set the directory where the compiled shaders (.spv) will be placed.
set(SHADER_OUTPUT_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/shaders")
file(MAKE_DIRECTORY ${SHADER_OUTPUT_DIR})

# 3. Find all shader files in the 'src/shaders' directory.
file(GLOB_RECURSE SHADER_SOURCES "src/shaders/*.comp" "src/shaders/*.vert" "src/shaders/*.frag")

set(SHADER_OUTPUT_FILES "")

# 4. Create a build rule for each shader found.
foreach(SHADER_SOURCE_FILE ${SHADER_SOURCES})
    # Get the filename without the extension (e.g., 'raytracer')
    get_filename_component(SHADER_NAME ${SHADER_SOURCE_FILE} NAME_WE)

    # Define the output file name (e.g., '.../build/shaders/raytracer.spv')
    set(SHADER_OUTPUT_FILE "${SHADER_OUTPUT_DIR}/${SHADER_NAME}.spv")

    # Add the output file to the list of shader output files.
    list(APPEND SHADER_OUTPUT_FILES ${SHADER_OUTPUT_FILE})

    # Add a custom command to CMake.
    # This tells the build system how to transform a .comp file into a .spv.
    add_custom_command(
        OUTPUT ${SHADER_OUTPUT_FILE}
        COMMAND ${GLSLC_COMPILER} -o ${SHADER_OUTPUT_FILE} ${SHADER_SOURCE_FILE}
        DEPENDS ${SHADER_SOURCE_FILE} # Only recompile if the source file changes
        COMMENT "Compiling shader: ${SHADER_SOURCE_FILE}"
    )
endforeach()

# 5. Create a custom target that depends on all shaders.
# This gives us an easy way to ensure all shaders are compiled.
add_custom_target(CompileShaders ALL DEPENDS ${SHADER_OUTPUT_FILES})

# 6. Add the shader output directory to the include path for the Prism library.
add_dependencies(Prism CompileShaders)