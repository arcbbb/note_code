
project(demo)

include_directories(${PROJECT_SOURCE_DIR}/../build/include)
link_directories(${PROJECT_SOURCE_DIR}/../build/lib)

cmake_minimum_required(VERSION 3.12)
set (CMAKE_CXX_STANDARD 11)

add_executable(conv conv.cpp)
target_link_libraries(demo Halide)
