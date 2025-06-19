file(GLOB_RECURSE TEST_SOURCES "src/*.cpp")
add_executable(runTests ${TEST_SOURCES})

target_link_libraries(runTests PRIVATE include gtest_main)
target_include_directories(runTests PRIVATE ${CMAKE_CURRENT_LIST_DIR}/include)

add_test(NAME UnitTests COMMAND runTests)