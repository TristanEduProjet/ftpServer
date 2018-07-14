cmake_minimum_required(VERSION 3.3.2)
project(eftp_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.c commands.c commands.h)
add_executable(eftp_server ${SOURCE_FILES})