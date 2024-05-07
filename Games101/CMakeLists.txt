cmake_minimum_required (VERSION 3.8)

set(ProjName "Game101")
project (${ProjName})

set(CMAKE_CXX_STANDARD 17)

if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()


set(BUILDPATH ${PROJECT_SOURCE_DIR}/build)
set(INCPATH /usr/include /usr/include/opencv4)
include_directories(${INCPATH} ${PROJECT_SOURCE_DIR})
#add_subdirectory(Lesson00)
file(GLOB lesson_directories RELATIVE ${PROJECT_SOURCE_DIR} "Lesson*")

foreach(lesson_dir ${lesson_directories})
    add_subdirectory(${lesson_dir})
endforeach()

#find_library(MYLIB NAMES libLesson00.so PATHS ${BUILDPATH})
file(GLOB DLL_FILES ${BUILDPATH}/*.so)
link_libraries(${BUILDPATH})

message(STATUS "Library is " ${DLL_FILES})

add_executable(${ProjName} games101.cpp)
target_link_libraries(${ProjName} ${DLL_FILES})


#if (CMAKE_VERSION VERSION_GREATER 3.12)
#  set_property(TARGET ${ProjName} PROPERTY CXX_STANDARD 20)
#endif()
# TODO: Add tests and install targets if needed.
