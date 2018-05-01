project(test)
set(CMAKE_CXX_STANDARD 14)

set(LLVM_HOME /media/code/git/llvm-master/install)
set(LLVM_DIR ${LLVM_HOME}/lib/cmake/llvm)
find_package(LLVM REQUIRED CONFIG)
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})

add_executable(cost main.cpp cost.cpp)

# check llvm components: llvm-config --components
llvm_map_components_to_libnames(llvm_libs support core irreader)

# Link against LLVM libraries
target_link_libraries(cost ${llvm_libs})
