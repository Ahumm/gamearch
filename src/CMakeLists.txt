cmake_minimum_required(VERSION 2.6)

set(
        SOURCES
        matrix.h
        vector.h
        includes.h
        globaldefs.h
)

add_library(matrix_vector ${SOURCES})

SET_TARGET_PROPERTIES(matrix_vector PROPERTIES LINKER_LANGUAGE C)