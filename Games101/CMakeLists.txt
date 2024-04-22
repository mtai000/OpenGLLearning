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
include_directories(${INCPATH})
#add_subdirectory(Lesson00)
file(GLOB lesson_directories RELATIVE ${PROJECT_SOURCE_DIR} "Lesson*")

foreach(lesson_dir ${lesson_directories})
    add_subdirectory(${lesson_dir})
endforeach()

#link_directories(${BUILDPATH})
#link_libraries(libLesson00.so libLesson01.so)

#set(EXECUTABLE_OUTPUT_PATH ${BUILDPATH})
#add_executable(${ProjName} games101.cpp)

# Add source to this project's executable.
#link_directories(${DLLPATH})

#file(GLOB DLL_FILES ${DLLPATH}/*.so)
#link_libraries(${DLL_FILES})

#set(EXECUTABLE_OUTPUT_PATH ${EXECPATH})
#add_executable (${ProjName} games101.cpp)

#target_link_libraries(${ProjName} "libLesson00.so")


#if (CMAKE_VERSION VERSION_GREATER 3.12)
#  set_property(TARGET ${ProjName} PROPERTY CXX_STANDARD 20)
#endif()
# TODO: Add tests and install targets if needed.
