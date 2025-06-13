add_library(Prism SHARED 
    src/vector.cpp
    src/point.cpp
    src/utils.cpp
    src/camera.cpp
    src/ray.cpp
    src/sphere.cpp
    src/color.cpp
    src/material.cpp
    src/plane.cpp
    src/environment.cpp
)

include(GenerateExportHeader)
generate_export_header(Prism)

target_include_directories(Prism PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
    "${CMAKE_CURRENT_BINARY_DIR}"
)

install(TARGETS Prism
    RUNTIME DESTINATION bin   # Para .dll no Windows
    LIBRARY DESTINATION lib   # Para .so no Linux, .dylib no macOS
    ARCHIVE DESTINATION lib   # Para .lib no Windows (bibliotecas de importação)
)