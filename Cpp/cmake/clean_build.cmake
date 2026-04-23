file(GLOB build_dirs LIST_DIRECTORIES TRUE ${CMAKE_BINARY_DIR}/*)
add_custom_target(clean_binary_dir
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${build_dirs}
)
if(EXISTS ${CMAKE_INSTALL_PREFIX})
    add_custom_target(clean_install_dir
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_INSTALL_PREFIX}
    )
endif()
