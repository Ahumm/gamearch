cmake_minimum_required(VERSION 2.6)

set (
    CMDLINE_SOURCES
    main.cpp
)

include_directories(. ${MATRIX_VECTOR_SOURCE_DIR}/lib)
link_directories(${MATRIX_VECTOR_BINARY_DIR}/lib)

add_executable(cube_transform ${CMDLINE_SOURCES})
target_link_libraries(cube_transform matrix_vector)
