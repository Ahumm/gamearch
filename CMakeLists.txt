cmake_minimum_required( VERSION 2.6 )
project(matrix_vector)

set(MATRIX_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(MATRIX_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR})

set(CMAKE_CXX_FLAGS "-g -static -std=c++0x")

add_subdirectory(lib)
add_subdirectory(mains)
