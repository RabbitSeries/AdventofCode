cmake_minimum_required(VERSION 3.10)

project(2024)

# Find all the .cpp files in the 2024 folder
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/2024/**/*.cpp")
file(COPY ${CMAKE_SOURCE_DIR}/2024 DESTINATION ${CMAKE_BINARY_DIR}
    FILES_MATCHING
    PATTERN "*.txt")
# Add the executable
add_executable(solution main.cc ${SOURCES})
