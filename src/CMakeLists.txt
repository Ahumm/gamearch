# Just some linking
include_directories(${GAMEARCH_SOURCE_DIR}/src)
link_directories(${GAMEARCH_BINARY_DIR}/src)

set(HW1_SOURCES hw1.cpp)

# Build hw1
#add_executable(hw1 WIN32 MACOSX_BUNDLE ${HW1_SOURCES})
add_executable(hw2 WIN32 MACOSX_BUNDLE hw2.cpp)
