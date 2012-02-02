# Just some linking
include_directories(${GAMEARCH_SOURCE_DIR}/src)
link_directories(${GAMEARCH_BINARY_DIR}/src)

# Build hw1
add_executable(hw1 WIN32 MACOSX_BUNDLE hw1.cpp)
