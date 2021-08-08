cmake_minimum_required(VERSION 3.10)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/3rd)

add_executable(Rasterizer2 main.cpp rasterizer.hpp rasterizer.cpp global.hpp Triangle.hpp Triangle.cpp)
target_link_libraries(Rasterizer2 ${OpenCV_LIBRARIES})
