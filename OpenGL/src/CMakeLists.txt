# CMakeList.txt : CMake project for OpenGL, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

set(ProjName "OpenGL")

project (${ProjName})
set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIR})
link_libraries(${OPENGL_LIBRARIES})

find_package(GLEW REQUIRED)
include_directories(${GLEW_INCLUDE_DIRS})
link_libraries(${GLEW_LIBRARIES})
message(${GLEW_LIBRARIES})

find_package(SOIL2 REQUIRED)
include_directories(${SOIL2_INCLUDE_DIR})
link_libraries(${SOIL2_LIBRARIES})

# Add source to this project's executable.
file(GLOB SRC ${PROJECT_SOURCE_DIR}/*.cpp ${PROJECT_SOURCE_DIR}/*.h)
list(APPEND SOURCES
"${PROJECT_SOURCE_DIR}/main.cpp"
)
add_executable (${ProjName} ${SRC})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET ${ProjName} PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(${ProjName} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES} glfw soil2)
file(COPY "${CMAKE_SOURCE_DIR}/GLSL" DESTINATION "${CMAKE_BINARY_DIR}")

# TODO: Add tests and install targets if needed.
