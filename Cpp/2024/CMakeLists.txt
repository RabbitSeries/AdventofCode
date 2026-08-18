add_executable(solution "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc")
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS
     "${CMAKE_CURRENT_SOURCE_DIR}/src/Day*/*.h")
list(
  SORT HEADERS
  COMPARE STRING
  ORDER ASCENDING)
foreach(HEADER_FILE ${HEADERS})
  message(STATUS "Seeing ${HEADER_FILE}")
  cmake_path(GET HEADER_FILE STEM _HEADER_STEM)
  cmake_path(GET HEADER_FILE FILENAME _HEADER_NAME)
  cmake_path(GET HEADER_FILE PARENT_PATH _HEADER_DIR)
  cmake_path(GET _HEADER_DIR FILENAME _HEADER_DIR)
  cmake_path(APPEND CMAKE_CURRENT_BINARY_DIR ${_HEADER_DIR} ${_HEADER_STEM}.cpp
             OUTPUT_VARIABLE SOLUTION_PATH)
  message(STATUS "Generating ${SOLUTION_PATH}")
  file(
    GENERATE
    OUTPUT ${SOLUTION_PATH}
    CONTENT "#include \"${_HEADER_DIR}/${_HEADER_NAME}\"")
  target_sources(solution PRIVATE ${SOLUTION_PATH})
endforeach()

target_include_directories(solution PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Multi-config generators like MSVC need resources copied into each config dir.
set(RESOURCES "${CMAKE_CURRENT_SOURCE_DIR}/resources")
file(
  GLOB_RECURSE INPUT_FILES
  RELATIVE ${RESOURCES}
  "${RESOURCES}/**/input.txt")
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
      add_custom_command(
        TARGET solution
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_DIR}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
                ${RESOURCES}/${INPUT_FILE} ${OUTPUT_DIR})
    endif()
  else()
    file(
      COPY "${CMAKE_CURRENT_SOURCE_DIR}/resources/"
      DESTINATION ${CMAKE_CURRENT_BINARY_DIR}
      FILES_MATCHING
      PATTERN input.txt)
  endif()
endforeach()

target_link_libraries(solution PRIVATE aoc_cpp_utils)

# These should remain relative for both cmake install (use CMAKE_INSTALL_PREFIX) and CPack (use CMAKE_PACKAGE_DIR/somecache.dir)
if(MSVC)
  set(INSTALL_DIR "AoC/Cpp/$<CONFIG>/2024")
else()
  set(INSTALL_DIR "AoC/Cpp/${CMAKE_BUILD_TYPE}/2024")
endif()

install(TARGETS solution DESTINATION ${INSTALL_DIR})

install(
  DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/resources/"
  DESTINATION ${INSTALL_DIR}
  FILES_MATCHING
  PATTERN input.txt)

set(CPACK_PACKAGE_NAME "solution")
set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION}")
set(CPACK_PACKAGE_FILE_NAME
    "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}")
include(CPack)
