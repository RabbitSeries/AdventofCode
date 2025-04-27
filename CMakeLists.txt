cmake_minimum_required(VERSION 3.10)

project(AdventOfCode2024)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -std=c++23 -Wall -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -std=c++23 -O2")

# add_compile_options(-O1)
add_subdirectory(2024)
