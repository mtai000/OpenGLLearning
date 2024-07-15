cmake_minimum_required(VERSION 3.10)
project(RayTracing)

set(CMAKE_CXX_STANDARD 14)

file(GLOB src *.cc *.h)

add_executable(RayTracing ${src})

