# Print size
macro(ulp_print_size ulp_app)
    set(CMAKE_SIZE "riscv32-esp-elf-size")
    add_custom_command(TARGET ${ulp_app} POST_BUILD
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMAND ${CMAKE_SIZE} "$<TARGET_FILE:${ulp_app}>"
        VERBATIM)
endmacro(ulp_print_size)

function(ulp_enable_rvfplib ulp_app)
    file(GLOB RVFPLIB_SOURCES "${CMAKE_CURRENT_FUNCTION_LIST_DIR}/../RVfplib/src/asm/ieee754/*.S")
    
    add_library(rvfp STATIC "${RVFPLIB_SOURCES}")

    if(${CONFIG_RVFPLIB_SIZE})
        target_compile_definitions(rvfp PRIVATE "-DSIZE=1")
    else()
        target_compile_definitions(rvfp PRIVATE "-DPERFORMANCE=1")
    endif()

    target_link_libraries(${ulp_app} rvfp)
endfunction(ulp_enable_rvfplib)
