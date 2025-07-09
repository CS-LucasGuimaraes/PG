# ===================================================================
# Prism Demo Application
#
# This CMakeLists.txt file defines the demo application that uses the Prism library.
# It sets up the executable target, links against the Prism library, and handles installation.
# ===================================================================

add_executable(prism_demo src/main.cpp)

target_link_libraries(prism_demo PRIVATE Prism)

# --- Installation ---
install(TARGETS prism_demo DESTINATION bin)

# This command is now much cleaner. It copies the 'data' directory
# from this subdirectory into the install directory.
install(DIRECTORY data/ DESTINATION .)

# --- Development/Debugging Support ---

# This is the key command that solves the debugging issue.
# It copies the 'data' directory into the build output directory
# *after* the executable is built.
add_custom_command(TARGET prism_demo POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/data
        $<TARGET_FILE_DIR:prism_demo>/data
    COMMENT "Copying data folder for debug/run..."
)