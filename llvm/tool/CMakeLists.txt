project(test)
set(CMAKE_CXX_STANDARD 14)

set(LLVM_HOME /media/code/git/llvm-master/install)
set(LLVM_DIR ${LLVM_HOME}/lib/cmake/llvm)
find_package(LLVM REQUIRED CONFIG)
add_definitions(${LLVM_DEFINITIONS})
include_directories(${LLVM_INCLUDE_DIRS})

# check llvm components: llvm-config --components
llvm_map_components_to_libnames(llvm_libs support core irreader)

#
# target: dump-bc-inst
#
set (target-name dump-bc-inst)
add_executable(${target-name} dump-bc-inst.cpp)

# Link against LLVM libraries
target_link_libraries(${target-name} ${llvm_libs})

#
# target: Hello
#
add_library(hello MODULE Hello.cpp)
SET(CMAKE_CXX_FLAGS "-Wall -fno-rtti")

#
# target: cl
#
set (target-name llvm-cl)
add_executable(${target-name} cl.cpp)

# Link against LLVM libraries
target_link_libraries(${target-name} ${llvm_libs})
