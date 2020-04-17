cmake_minimum_required (VERSION 2.6)
project (sysop_lab1_libraries C)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c11 -O0")
set(CMAKE_C_STANDARD 11)

include_directories(lib)

add_executable(main_static src/main.c)
add_executable(main_shared src/main.c)
add_executable(main_module src/main_module.c)


# static, shared and dynamically linked versions of a dummy library
# these libraries could be provided from outside sources
# Then you would just link already compiled *.a or *.so files
add_library(dummy_static STATIC lib/dummy.c)
add_library(dummy_shared SHARED lib/dummy.c)
add_library(dummy_module MODULE lib/dummy.c)

# link libraries
target_link_libraries(main_static dummy_static)
target_link_libraries(main_shared dummy_shared)
target_link_libraries(main_module ${CMAKE_DL_LIBS})