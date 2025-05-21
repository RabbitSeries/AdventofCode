cmake_minimum_required(VERSION 3.10)

project(2024)
# Find all the .cpp files in the 2024 folder
file(GLOB_RECURSE SOURCES "${CMAKE_SOURCE_DIR}/2024/**/*.cpp")
message("Copying input resources")
file(GLOB_RECURSE INPUT_FILES "${CMAKE_CURRENT_SOURCE_DIR}/**/input.txt")
file(COPY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION ${CMAKE_BINARY_DIR}
    FILES_MATCHING
    PATTERN "input.txt")
message("Copying input resources -- done")
# Add the executable

# add_compile_options(-O1)
add_executable(solution main.cc ${SOURCES})

target_compile_options(solution PRIVATE
    $<$<CONFIG:Debug>:-g>
    $<$<CONFIG:Release>:-O3>
    $<$<CONFIG:RelWithDebInfo>: -g -O2 -DNDEBUG>
)

install(TARGETS solution DESTINATION "AoC/2024")
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} DESTINATION "AoC"
    FILES_MATCHING
    PATTERN "input.txt")
