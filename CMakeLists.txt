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
add_definitions("-g -DGLEW_STATIC -static")
#add_subdirectory(src)

set(EXECUTABLE_NAME "hw5-2")

add_executable(${EXECUTABLE_NAME} WIN32 MACOSX_BUNDLE src/hw5-2.cpp)

# Detect and add SFML
set(SFMLDIR "C:/MinGW/")
find_package(SFML 2 REQUIRED system window graphics network audio)
target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})

#find_package(OpenGL REQUIRED)
#include_directories(${OPENGL_INCLUDE_DIR})
#TARGET_LINK_LIBRARIES(${EXECUTABLE_NAME} ${OPENGL_LIBRARIES})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_PATH})
target_link_libraries(${EXECUTABLE_NAME} ${GLEW_LIBRARY})
#target_link_libraries(${EXECUTABLE_NAME} glew)
target_link_libraries(${EXECUTABLE_NAME} opengl32)

