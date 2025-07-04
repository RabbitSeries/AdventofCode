# Find all the .cpp files in the 2024 folder
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/**/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/**/*.cc")
message(STATUS "Copying input resources")

# For debug build
file(GLOB_RECURSE AOC_INPUTS LIST_DIRECTORIES FALSE input.txt)
message(STATUS "Copying input resources -- done")
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/resources/" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
    FILES_MATCHING
    PATTERN
    input.txt
)
add_executable(solution ${SOURCES})

if(MSVC)
    target_compile_options(solution PRIVATE
        $<$<CONFIG:Debug>:/Zi>
        $<$<CONFIG:Release>:/O2>
        $<$<CONFIG:RelWithDebInfo>: /Zi /Od>
    )
else()
    target_compile_options(solution PRIVATE
        $<$<CONFIG:Debug>:-g>
        $<$<CONFIG:Release>:-O3>
        $<$<CONFIG:RelWithDebInfo>: -g -O2 -DNDEBUG>
    )
endif()

target_link_libraries(solution PRIVATE aoc_cpp_utils)

install(TARGETS solution DESTINATION "AoC/Cpp/2024")

# foreach(file ${AOC_INPUTS})
# cmake_path(GET file PARENT_PATH parent_dir)
# cmake_path(RELATIVE_PATH parent_dir BASE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/resources OUTPUT_VARIABLE rel_path)
# file(COPY ${file} DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/${rel_path}")
# endforeach()
install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources/" DESTINATION "AoC/Cpp/2024" FILES_MATCHING PATTERN input.txt)
