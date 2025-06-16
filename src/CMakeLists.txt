add_executable(${PROJECT_NAME})

target_sources(${PROJECT_NAME} PRIVATE main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE vendor)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)


if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${PROJECT_SOURCE_DIR}/data
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/data"
        COMMENT "Copying data for Debug build..."
    )
endif()
