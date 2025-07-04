cmake_minimum_required(VERSION 3.28)

project(AoC_Cpp LANGUAGES CXX VERSION 1.0.1)

if(MSVC)
    add_compile_options(/std:c++latest /Wall)
else()
    add_compile_options(-std=c++23 -Wall)
endif()

# include(${CMAKE_CURRENT_LIST_DIR}/cpp/cmake/glob_projects.cmake)
# glob_projects(${CMAKE_CURRENT_SOURCE_DIR}/cpp 2 "[0-9]+/main" res FALSE)
# message(STATUS "Found solutions: ${res}")
# glob_projects(${CMAKE_CURRENT_SOURCE_DIR}/Java 2 "[0-9]+/main" res FALSE)
# message(STATUS "Found solutions: ${res}")
add_subdirectory(Cpp/libs)
add_subdirectory(Cpp/2024)

include(Cpp/cmake/clean_build.cmake)