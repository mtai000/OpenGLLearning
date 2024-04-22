cmake_minimum_required (VERSION 3.8)
set (LESSON Lesson01)
project (${LESSON})

find_package(Eigen3 REQUIRED)
include_directories(EIGEN3_INCLUDE_DIR)

aux_source_directory(./ SRC)
include_directories(${INCPATH})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BUILDPATH})

add_library(${LESSON} SHARED ${SRC})
find_package(OpenCV REQUIRED)
target_link_libraries(${LESSON} ${OpenCV_LIBRARIES})