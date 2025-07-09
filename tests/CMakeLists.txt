# ===================================================================
# Prism Unit Tests
#
# This CMakeLists.txt file sets up the unit tests for the Prism library.
# It defines the test runner executable, links it against the Prism library,
# and registers the tests with CTest.
# ===================================================================

# Collect all test source files.
file(GLOB_RECURSE TEST_SOURCES "src/*.cpp")

# Define the test runner executable.
add_executable(runTests ${TEST_SOURCES})

# Add the local `include` directory for test helpers.
target_include_directories(runTests PRIVATE include)

# Link the test runner against the library being tested (Prism)
# and the GoogleTest framework.
target_link_libraries(runTests PRIVATE Prism GTest::gtest_main)

# Register the executable with CTest.
add_test(NAME UnitTests COMMAND runTests)