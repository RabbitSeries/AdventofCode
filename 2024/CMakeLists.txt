cmake_minimum_required(VERSION 3.10)

project(2024)
# Find all the .cpp files in the 2024 folder
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/2024/**/*.cpp")
message("Copying input resources")
file(COPY ${CMAKE_SOURCE_DIR}/2024 DESTINATION ${CMAKE_BINARY_DIR}
FILES_MATCHING
PATTERN "*.txt")
message("Copying input resources -- done")
# Add the executable

# add_compile_options(-O1)
add_executable(solution main.cc ${SOURCES})
