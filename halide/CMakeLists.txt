project(demo)

include_directories(${PROJECT_SOURCE_DIR}/../build/include)
link_directories(${PROJECT_SOURCE_DIR}/../build/lib)
link_directories(/usr/local/bin)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(conv conv.cpp)
target_link_libraries(conv Halide)
