# CMakeList.txt: GAMES101 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

include_directories(${PROJECT_SOURCE_DIR}/3rd)
add_executable (
    ps0 
    "main.cpp"
    "main.h"
)

enable_testing()

add_executable(
  hello_test
  test.cc
 "main.h"
)

target_link_libraries(
  hello_test
  gtest_main
)

include(GoogleTest)
gtest_discover_tests(hello_test)