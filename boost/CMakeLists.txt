project(test)
set(CMAKE_CXX_STANDARD 14)

find_package(Boost COMPONENTS program_options REQUIRED )
include_directories(${Boost_INCLUDE_DIRS})
#
# target: boost-po
#
set (target-name boost-po)
add_executable(${target-name} boost-po.cpp)
target_link_libraries(${target-name} ${Boost_LIBRARIES})
