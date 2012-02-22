cmake_minimum_required(VERSION 2.6)
project(GAMEARCH)

# Set location to output binaries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${GAMEARCH_SOURCE_DIR}/bin)
set(CMAKE_INCLUDE_PATH ${GAMEARCH_SOURCE_DIR}/includes)

include_directories(${GAMEARCH_SOURCE_DIR}/include)

# ADD GLEW AND SFML OR GLFW STUFF HERE OR DEEPER

# These next two are pretty much the same
#set(CMAKE_CXX_FLAGS "-g -std=c++0x")
#add_definitions("-g -std=c++0x")
add_definitions("-g")
add_subdirectory(src)
