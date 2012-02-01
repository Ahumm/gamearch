cmake_minimum_required(VERSION 2.6)
project(GAMEARCH)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${GAMEARCH_SOURCE_DIR}/bin)

set(CMAKE_CXX_FLAGS "-g -static -std=c++0x")

add_subdirectory(src)