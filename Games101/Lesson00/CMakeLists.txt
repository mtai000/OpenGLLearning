cmake_minimum_required (VERSION 3.8)
set (PROJNAME Lesson00)
project (${PROJNAME})

find_package(Eigen3 REQUIRED)
include_directories(EIGEN3_INCLUDE_DIR)

aux_source_directory(./ SRC)
include_directories(${INCPATH})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${BUILDPATH})
add_library(${PROJNAME} SHARED ${SRC})
