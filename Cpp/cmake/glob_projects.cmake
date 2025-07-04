function(glob_projects root_dir depth pattern result_variable append)
    file(REAL_PATH ${root_dir} root_abs EXPAND_TILDE)

    function(glob_helper current_dir cur_depth out_var)
        # Ceate another variable to reference the value of parent_scope result variable
        # The following is ambiguous:
        # glob_helper ( .. ..  out_var)
        # set(${out_var} ${${out_var}} PARENT_SCOPE)
        set(current_res "${${out_var}}")

        if(${cur_depth} EQUAL ${depth})
            file(RELATIVE_PATH relative_dir ${root_abs} ${current_dir})

            if(${relative_dir} MATCHES ${pattern} AND EXISTS "${current_dir}/../CMakeLists.txt")
                # After each recursive return, refer to the former value first,
                # then assign the value to the parent varaible
                set(${out_var} "${current_res};${current_dir}" PARENT_SCOPE)
            endif()

            return()
        endif()

        file(GLOB entries LIST_DIRECTORIES TRUE ${current_dir}/*)

        foreach(entry ${entries})
            if(IS_DIRECTORY ${entry})
                math(EXPR next_depth "${cur_depth}+1")

                # Pass each scope's result variable name to the next scope
                glob_helper(${entry} ${next_depth} current_res)
            endif()
        endforeach()

        set(${out_var} ${current_res} PARENT_SCOPE) # A
    endfunction()

    glob_helper(${root_abs} 0 glob_res)

    if(${append})
        list(APPEND glob_res "${${result_variable}}")
    endif()

    set(${result_variable} ${glob_res} PARENT_SCOPE)
endfunction()
