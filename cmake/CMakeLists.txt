
#
# Target creation: add_library, add_executable, add_custom_target
#     add_library(foo STATIC ...)
#     add_library(foo SHARED ...)
# target has properties
#     set_target_properties
#     get_target_property
#     target_link_libraries

# Source files
#     COMPILE_FLAGS: -D -I specific to this source file.
#     GENERATED
# set_source_files_properties
# get_source_files_property

# Variables
#   new variables created in the child scope,
#   will not impact the parent scope
#     set(foo 1)
#     message (${foo})

# Cache Entries
#     option (ENABLE_TEST "Enable unit-test")
#     set (ENABLE_TEST ON CACHE BOOL "Enable unit-test")
#     set (TEST_ITEM "TEST_FOO" CACHE STRING "Select an item for test")
#     set_property (CACHE TEST_ITEM PROPERTY STRINGS "TEST_FOO" "TEST_BAR")

# Control statement
#     if(${foo} LESS 2)
#         set(foo 2)
#     else()
#         set(foo 3)
#     endif()
#         set (items apple orange pear)
#         foreach (item ${items})
#             message (${item})
#         endforeach ()

#
# Example: find package
#
#set(LLVM_DIR ${LLVM_HOME}/share/llvm/cmake)
#find_package(LLVM REQUIRED CONFIG)
#add_definitions(${LLVM_DEFINITIONS})
#include_directories(${LLVM_INCLUDE_DIRS})
#link_directories(${LLVM_LIBRARY_DIRS})

# find_package(<package>)
# REQUIRED: stop processing if package cannot be found
# Module mode - Search for Find<package>.cmake in CMAKE_MODULE_PATH
# Config mode - Search for <package>-Config.cmake in <package>_DIR
# <package>_FOUND
