function(oe_install src)
    install(DIRECTORY ${src} DESTINATION "Engine" OPTIONAL PATTERN "*.md" EXCLUDE)
    install(DIRECTORY "../${src}" DESTINATION "." OPTIONAL)
endfunction()

function(oe_optimize_target target)
    if(OE_USE_AVX2)
        target_compile_options(${target} PRIVATE
            $<IF:$<CXX_COMPILER_ID:MSVC>,/arch:AVX2,-march=haswell>
        )
        target_compile_definitions(${target} PRIVATE OE_MODULE_USE_AVX2)
    endif()

    target_compile_options(${target} PRIVATE
        $<IF:$<CXX_COMPILER_ID:MSVC>, /MP /fp:fast, -ffast-math>
    )

    set_property(TARGET ${target} PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endfunction()

function(oe_setup_target target)
    oe_optimize_target(${target})

    target_compile_options(${target} PRIVATE
        $<IF:$<CXX_COMPILER_ID:MSVC>,
            /wd4251 # 'type' : class 'type1' needs to have dll-interface to be used by clients of class 'type2'
            /wd4275 # non - DLL-interface class 'class_1' used as base for DLL-interface class 'class_2'
            /W4 /WX

            , # Clang/GCC
            -Wall -Wextra -pedantic -Werror
        >

        $<$<CXX_COMPILER_ID:Clang>:-Wno-gnu-zero-variadic-macro-arguments>
    )

    if(NOT MSVC)
        target_link_options(${target} PRIVATE -Wl$<COMMA>-rpath='$\{ORIGIN}')
    endif()
endfunction()

function(add_module name)
    set(target ${PROJECT_NAME}-${name})

    file(GLOB_RECURSE sources CONFIGURE_DEPENDS "Private/*.cpp")
    add_library(${target} SHARED ${sources})
    target_include_directories(${target} PUBLIC "Public")

    string(TOUPPER ${name}_API api)
    target_compile_definitions(${target} PRIVATE ${api}=OE_EXPORT INTERFACE ${api}=OE_IMPORT)

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Public/pch.hpp")
        target_precompile_headers(${target} PUBLIC Public/pch.hpp)
    endif()
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Private/pch.hpp")
        target_precompile_headers(${target} PRIVATE Private/pch.hpp)
    endif()

    oe_setup_target(${target})
endfunction()

function(oe_add_program name)
    file(GLOB_RECURSE sources CONFIGURE_DEPENDS "Private/*.cpp")
    add_executable(${name} ${sources})
    target_include_directories(${name} PUBLIC "Public")

    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Public/pch.hpp")
        target_precompile_headers(${name} PUBLIC Public/pch.hpp)
    endif()
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/Private/pch.hpp")
        target_precompile_headers(${name} PRIVATE Private/pch.hpp)
    endif()

    oe_setup_target(${name})
endfunction()

macro(oe_define_switch module name)
    if(${name})
        target_compile_definitions(${PROJECT_NAME}-${module} PUBLIC ${name})
    endif()
endmacro()

macro(oe_check_mutex opt1 opt2)
    if(${opt1} AND ${opt2})
        message(FATAL_ERROR "${opt1} and ${opt2} are mutually exclusive")
    endif()
endmacro()

function(oe_check_only_one)
    set(lastTrueVar "")
    foreach(varName ${ARGV})
        if(${varName})
            if(lastTrueVar)
                message(FATAL_ERROR
                    "Only one of the following variables can be true: ${ARGV}\n"
                    "But these are set to true: ${lastTrueVar} ${varName}")
            else()
                set(lastTrueVar ${varName})
            endif()
        endif()
    endforeach()

    if(NOT lastTrueVar)
        message(FATAL_ERROR "One of the following variables must be true: ${ARGV}")
    endif()
endfunction()

function(oe_download_package name)
    set(cmake_dir "${PROJECT_SOURCE_DIR}/CMake/AutoInstall")
    set(src_dir "${PROJECT_SOURCE_DIR}/Source/ThirdParty/${name}")
    set(build_dir "${CMAKE_BINARY_DIR}/ThirdParty/${name}-build")
    set(download_dir "${CMAKE_BINARY_DIR}/ThirdParty/${name}-download")
    configure_file("${cmake_dir}/${name}.cmake" "${download_dir}/CMakeLists.txt" @ONLY)
    
    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY "${download_dir}"
    )
    if(result)
        message(FATAL_ERROR "CMake step for ${name} failed: ${result}")
    endif()

    execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY "${download_dir}"
    )
    if(result)
        message(FATAL_ERROR "Build step for ${name} failed: ${result}")
    endif()

    if(EXISTS "${cmake_dir}/${name}-build.cmake")
        configure_file("${cmake_dir}/${name}-build.cmake" "${src_dir}/CMakeLists.txt" @ONLY)
    endif()

    if(EXISTS "${src_dir}/CMakeLists.txt")
        add_subdirectory("${src_dir}" "${build_dir}" EXCLUDE_FROM_ALL)
    endif()
endfunction()

function(oe_find_package name)
    if(${ARGC} GREATER 1)
        set(version ${ARGV1})
    endif()

    if(OE_DEPS_FORCE_EXTERNAL)
        if (version)
            find_package(${name} ${version} REQUIRED)
        else()
            find_package(${name} REQUIRED)
        endif()
    elseif(NOT OE_DEPS_FORCE_INTERNAL)
        if (version)
            find_package(${name} ${version} QUIET)
        else()
            find_package(${name} QUIET)
        endif()
    endif()

    if(${name}_FOUND)
        if(version)
            message(STATUS "Found ${name} from system (Required is at least version ${version})")
        else()
            message(STATUS "Found ${name} from system")
        endif()
    else()
        if(NOT OE_DEPS_FORCE_INTERNAL)
            if(version)
                message(STATUS "Could not find ${name} that is compatible with requested version ${version}. Downloading...")
            else()
                message(STATUS "Could not find ${name} from system. Downloading...")
            endif()
        endif()
        oe_download_package(${name})
    endif()
endfunction()

function(oe_find_path auto_install_name out_var filename)
    if (NOT OE_DEPS_FORCE_INTERNAL)
        find_path(${out_var} ${filename})
    endif()

    if(${out_var} STREQUAL ${out_var}-NOTFOUND)
        if(OE_DEPS_FORCE_EXTERNAL)
            message(FATAL_ERROR "Could not find ${filename} from system.")
        elseif(NOT OE_DEPS_FORCE_INTERNAL)
            message(STATUS "Could not find ${filename} from system. Downloading...")
        endif()

        oe_download_package(${auto_install_name})
    else()
        message(STATUS "Found ${filename} from system")
    endif()
endfunction()

function(oe_add_subdirectories)
    file(GLOB entries ${CMAKE_CURRENT_SOURCE_DIR}/*)
    foreach(entry ${entries})
        if(IS_DIRECTORY ${entry} AND EXISTS ${entry}/CMakeLists.txt)
            add_subdirectory(${entry})
        endif()
    endforeach()
endfunction()

function(oe_link_sdl2 module)
    target_link_libraries(${PROJECT_NAME}-${module}
      PRIVATE
	    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,${SDL2_LIBRARIES}>
    )

    target_include_directories(${PROJECT_NAME}-${module}
      PRIVATE
	    ${SDL2_INCLUDE_DIRS}
    )
endfunction()
