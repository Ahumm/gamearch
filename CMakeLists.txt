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

IF(WIN32)
	set(SFMLDIR "C:/MinGW/")
ENDIF()
find_package(SFML 2 REQUIRED system window graphics network audio)

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_PATH})

set(PROGRAM_NAME "hw6" "hw6-2")

foreach(program ${PROGRAM_NAME})
	add_executable(${program} WIN32 MACOSX_BUNDLE src/${program}.cpp)

	# Detect and add SFML
	target_link_libraries(${program} ${SFML_LIBRARIES})

	target_link_libraries(${program} ${GLEW_LIBRARY})

	IF(WIN32)
		target_link_libraries(${program} opengl32)
	ELSE()
		find_package(OpenGL REQUIRED)
		include_directories(${OPENGL_INCLUDE_DIR})
		TARGET_LINK_LIBRARIES(${program} ${OPENGL_LIBRARIES})
	ENDIF()
endforeach(program)

