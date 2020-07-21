# Source/Runtime

function(add_engine_module name public_libs private_libs)
	set(target ${PROJECT_NAME}-${name})
	file(GLOB_RECURSE sources "Private/*.cpp")
	add_library(${target} SHARED ${sources})

	target_link_libraries(${target} PUBLIC ${public_libs} PRIVATE ${private_libs})
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

function(oe_find_package name version)
	find_package(${name} ${version} EXACT QUIET)
	if(${name}_FOUND)
		message(STATUS "Found ${name} ${version} from system")
	else()
		message(STATUS "${name} ${version} not found. Downloading...")
		configure_file("${PROJECT_SOURCE_DIR}/cmake/${name}.cmake" "${name}-download/CMakeLists.txt")

		add_subdirectory(
			${CMAKE_CURRENT_BINARY_DIR}/${name}-src
			${CMAKE_CURRENT_BINARY_DIR}/${name}-build
			EXCLUDE_FROM_ALL
		)
	endif()
endfunction()

add_subdirectory(Core)
add_subdirectory(Engine)
add_subdirectory(Launch)
add_subdirectory(Renderer)
