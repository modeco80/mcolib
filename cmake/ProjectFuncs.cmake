function(mco_target target)
        # Set binary products to output in the build directory for easier access
        set_target_properties(
                ${target} PROPERTIES
                RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}"
        )

        target_include_directories(
                ${target} PUBLIC ${PROJECT_SOURCE_DIR}/include
        )

        # Require C++23
        target_compile_features(${target} PUBLIC cxx_std_23)
endfunction()

function(mco_set_alternate_linker)
        find_program(LINKER_EXECUTABLE ld.${MCO_LINKER} ${MCO_LINKER})
        if(LINKER_EXECUTABLE)
                message(STATUS "Using ${MCO_LINKER} as argument to -fuse-ld=")
        else()
                message(FATAL_ERROR "Linker ${MCO_LINKER} does not exist on your system. Please specify one which does or omit this option from your configure command.")
        endif()
endfunction()

# FIXME: Better MSVC detection
if(NOT WIN32)
    # set the default linker based on compiler id, if one is not provided
    # This is provided so that it can be overridden
    if(NOT MCO_LINKER AND "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
            set(MCO_LINKER "lld")
    elseif(NOT MCO_LINKER)
            set(MCO_LINKER "bfd")
    endif()

    mco_set_alternate_linker()
endif()
