cmake_minimum_required(VERSION 2.6)
project(interpretator)

set (SOURCES "./sources/main.cpp" "./sources/hart_t.cpp" "./sources/instruction_t.cpp" "./sources/memory_t.cpp" "./sources/common.cpp")
set (HEADERS "./inlcude/common.hpp" "./include/hart_t.hpp" "./inlcude/instruction_t.hpp" "./include/memory_t.hpp")

set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -Wall -Wextra")

add_executable(interpretator ${SOURCES})