cmake_minimum_required(VERSION 2.6)
project(FAST_VEC)

add_definitions("-g -Wall")

set(
    SOURCES
    test.cpp
    fast_vec.cpp
)

add_executable(vec_test ${SOURCES})
