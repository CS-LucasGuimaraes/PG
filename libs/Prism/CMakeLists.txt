include(FetchContent)
FetchContent_Declare(
  yaml-cpp
  GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
  GIT_TAG        0.8.0
)
FetchContent_MakeAvailable(yaml-cpp)

file(GLOB_RECURSE PRISM_SOURCES CONFIGURE_DEPENDS "src/*.cpp")
add_library(Prism SHARED ${PRISM_SOURCES})

include(GenerateExportHeader)
generate_export_header(Prism)

target_include_directories(Prism PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_BINARY_DIR}"
)

target_link_libraries(Prism PUBLIC yaml-cpp)

install(TARGETS Prism
    RUNTIME DESTINATION bin   # Para .dll no Windows
    LIBRARY DESTINATION lib   # Para .so no Linux, .dylib no macOS
    ARCHIVE DESTINATION lib   # Para .lib no Windows (bibliotecas de importação)
)