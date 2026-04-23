file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/**/*.cc" "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc")
add_executable(solution ${SOURCES})

# Multi-config generators like MSVC need resources copied into each config dir.
set(RESOURCES "${CMAKE_CURRENT_SOURCE_DIR}/resources")
file(GLOB_RECURSE INPUT_FILES RELATIVE ${RESOURCES} "${RESOURCES}/**/input.txt")
foreach(INPUT_FILE IN LISTS INPUT_FILES)
    if(MSVC)
        if(EXISTS ${RESOURCES}/${INPUT_FILE})
            # file(GENERATE) is not plain copy, it will perform expansion in input file or content.
            # file(GENERATE OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${INPUT_FILE}" INPUT "${RESOURCES}/${INPUT_FILE}")
            # This will perform copy for all configs.
            # foreach(config LISTS CMAKE_CONFIGURATION_TYPES)
            #     configure_file("${RESOURCES}/${INPUT_FILE}" "${CMAKE_CURRENT_BINARY_DIR}/${config}/${INPUT_FILE}" COPYONLY)
            # endforeach()
            cmake_path(GET INPUT_FILE PARENT_PATH INPUT_DIR)
            set(OUTPUT_DIR ${CMAKE_CURRENT_BINARY_DIR}/$<CONFIG>/${INPUT_DIR})
            add_custom_command(TARGET solution POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_DIR}
                COMMAND ${CMAKE_COMMAND} -E copy_if_different ${RESOURCES}/${INPUT_FILE} ${OUTPUT_DIR}
            )
        endif()
    else()
        file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/resources/" DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
            FILES_MATCHING
            PATTERN
            input.txt
        )
    endif()
endforeach()

target_link_libraries(solution PRIVATE aoc_cpp_utils)

install(TARGETS solution DESTINATION "AoC/Cpp/2024")

install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources/" DESTINATION "AoC/Cpp/2024" FILES_MATCHING PATTERN input.txt)

set(CPACK_PACKAGE_NAME "solution")
set(CPACK_PACKAGE_VENDOR "RabbitX")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}")
include(CPack)

