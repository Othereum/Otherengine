function(oe_setup_target target)
    set_target_properties(${target} PROPERTIES
        CXX_STANDARD 20
        RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}/Binaries>
        LIBRARY_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}/Binaries>
    )

    target_compile_options(${target} PUBLIC
        $<IF:$<CXX_COMPILER_ID:MSVC>,
            /MP /fp:fast /arch:AVX2 /GL /W4 /WX,
            -ffast-math -march=haswell -flto -Wall -Wextra -pedantic -Werror>)

    target_link_options(${target} PUBLIC 
        $<IF:$<CXX_COMPILER_ID:MSVC>,
            /LTCG,
            -flto -Wl$<COMMA>-rpath='$\{ORIGIN}'>)
endfunction()
