# CMakeList.txt: GAMES101 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.14)

include_directories(${PROJECT_SOURCE_DIR}/3rd)
add_executable (
    exercise1 
    "main.cpp"
)

enable_testing()

add_executable(
    trans2d_test
    "test_transformate2d.cc"
    "transformate2d.h"
)

target_link_libraries(
    trans2d_test
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(trans2d_test)