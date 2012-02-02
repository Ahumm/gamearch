cmake_minimum_required(VERSION 2.6)
project(GAMEARCH)

# Set location to output binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${GAMEARCH_SOURCE_DIR}/bin)

# These next two are pretty much the same
#set(CMAKE_CXX_FLAGS "-g -std=c++0x")
add_definitions("-g -std=c++0x")

add_subdirectory(src)
