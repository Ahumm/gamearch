# Just some linking
include_directories(${GAMEARCH_SOURCE_DIR}/src)
link_directories(${GAMEARCH_BINARY_DIR}/src)

# Build hw binaries
add_executable(hw2 WIN32 MACOSX_BUNDLE hw2.cpp)
add_executable(hw3 WIN32 MACOSX_BUNDLE hw3.cpp)
