function(oe_install src)
	install(DIRECTORY ${src} DESTINATION "Engine" OPTIONAL PATTERN "*.md" EXCLUDE)
	install(DIRECTORY "../${src}" DESTINATION "." OPTIONAL)
endfunction()

function(oe_setup_target target)
	get_target_property(target_type ${target} TYPE)

	if(OE_USE_AVX2)
		target_compile_options(${target} PRIVATE
			$<IF:$<CXX_COMPILER_ID:MSVC>,/arch:AVX2,-march=haswell>
		)
		target_compile_definitions(${target} PRIVATE OE_MODULE_USE_AVX2)
	endif()

	target_compile_options(${target} PRIVATE
		$<IF:$<CXX_COMPILER_ID:MSVC>,
			$<$<CONFIG:Release>:/GL> /MP /fp:fast /W4 /WX /wd4251 /wd4275,
			$<$<CONFIG:Release>:-flto> -ffast-math -Wall -Wextra -pedantic -Werror>
		$<$<CXX_COMPILER_ID:Clang>:-Wno-gnu-zero-variadic-macro-arguments>
	)

	target_link_options(${target} PRIVATE
		$<IF:$<CXX_COMPILER_ID:MSVC>,
			$<$<CONFIG:Release>:/LTCG>,
			$<$<CONFIG:Release>:-flto> -Wl$<COMMA>-rpath='$\{ORIGIN}'>
	)
endfunction()

function(add_module name)
	set(target ${PROJECT_NAME}-${name})

	file(GLOB_RECURSE sources "Private/*.cpp")
	add_library(${target} SHARED ${sources})
	target_include_directories(${target} PUBLIC "Public")

	string(TOUPPER ${name}_API api)
	target_compile_definitions(${target} PRIVATE ${api}=OE_EXPORT INTERFACE ${api}=OE_IMPORT)

	oe_setup_target(${target})
endfunction()

function(add_switch module name default description)
    option(${name} ${description} ${default})
	if(${name})
		target_compile_definitions(${PROJECT_NAME}-${module} PUBLIC ${name})
	endif()
endfunction()

macro(oe_check_mutex opt1 opt2)
	if(${opt1} AND ${opt2})
		message(FATAL_ERROR "${opt1} and ${opt2} are mutually exclusive")
	endif()
endmacro()

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

	add_subdirectory("${src_dir}" "${build_dir}" EXCLUDE_FROM_ALL)
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
