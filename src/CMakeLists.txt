cmake_minimum_required(VERSION 2.6)

include_directories(${GAMEARCH_SOURCE_DIR}/src)
link_directories(${GAMEARCH_BINARY_DIR}/src})

set(
        HW1_SOURCES
        main.cpp
)

add_executable(hw1 ${HW1_SOURCES})