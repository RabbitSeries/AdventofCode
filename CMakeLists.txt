cmake_minimum_required(VERSION 3.28)

project(
  AoC_Cpp
  LANGUAGES CXX
  VERSION 0.1.0)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 23)

include(CheckCXXCompilerFlag)

if(MSVC)
  add_compile_options(/W4 /arch:AVX2 $<$<CONFIG:Debug>:/Od>
                      $<$<CONFIG:Release>:/O2>)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  check_cxx_compiler_flag("-mavx2" COMPILER_HAS_AVX2_FLAG)
  add_compile_options(-Wall -Wextra -Wpedantic -mavx2 $<$<CONFIG:Debug>:-g>
                      $<$<CONFIG:Release>:-O3>)
endif()

# include(${CMAKE_CURRENT_LIST_DIR}/cpp/cmake/glob_projects.cmake)
# glob_projects(${CMAKE_CURRENT_SOURCE_DIR}/cpp 2 "[0-9]+/main" res FALSE)
# message(STATUS "Found solutions: ${res}")
# glob_projects(${CMAKE_CURRENT_SOURCE_DIR}/Java 2 "[0-9]+/main" res FALSE)
# message(STATUS "Found solutions: ${res}")
add_subdirectory(Cpp/libs)
add_subdirectory(Cpp/2024)

include(Cpp/cmake/clean_build.cmake)
