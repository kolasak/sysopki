cmake_minimum_required (VERSION 2.6)
project (sysop_lab2_getopt_getrandom_nftw_readdir C)

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c11")
set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(getopt_example src/getopt_example.c)
add_executable(getrandom_example src/getrandom_example.c)
add_executable(nftw_example src/nftw_example.c)
add_executable(readdir_example src/readdir_example.c)

