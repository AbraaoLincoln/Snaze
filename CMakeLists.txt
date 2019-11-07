cmake_minimum_required(VERSION 3.5)
project(snake)

set(GCC_COMPILE_FLAGS "-Wall")
set(GCC_COMPILE_FLAGS "-std=c++11")
set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COMPILE_FLAGS}" )

include_directories(include)

file(GLOB SOURCES "src/*.cpp")

add_executable(snaze ${SOURCES})
