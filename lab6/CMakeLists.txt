cmake_minimum_required(VERSION 2.6)
project(sysop_lab6_queues C)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c11")
set(CMAKE_C_STANDARD 11)


include_directories(include)
include_directories(include/lib)


set(PREFIX "src/system_V")
add_executable(systemV_server ${PREFIX}/server.c ${PREFIX}/server_routines.c ${PREFIX}/chat.c)
add_executable(systemV_client ${PREFIX}/client.c ${PREFIX}/client_routines.c ${PREFIX}/chat.c)

target_link_libraries(systemV_server -pthread)
target_link_libraries(systemV_client -pthread)


set(PREFIX "src/POSIX")
add_executable(POSIX_server ${PREFIX}/server.c ${PREFIX}/chat.c)
add_executable(POSIX_client ${PREFIX}/client.c ${PREFIX}/chat.c)

target_link_libraries(POSIX_server -lrt -pthread)
target_link_libraries(POSIX_client -lrt -pthread)
