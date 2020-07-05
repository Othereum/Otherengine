# Source/Runtime

function(add_engine_module name public_libs private_libs)
	set(target ${PROJECT_NAME}-${name})
	file(GLOB_RECURSE sources "Private/*.cpp")
	add_library(${target} SHARED ${sources})
	target_link_libraries(${target} PUBLIC ${public_libs} PRIVATE ${private_libs})
	target_include_directories(${target} PUBLIC "Public")
	target_compile_definitions(${target} PRIVATE OE_BUILD)
	oe_setup_target(${target})
endfunction()

function(add_switch module name default description)
    set(${name} ${default} CACHE BOOL ${description})
	if(${name})
		target_compile_definitions(${PROJECT_NAME}-${module} PUBLIC ${name})
	endif()
endfunction()

add_subdirectory(Core)
add_subdirectory(Engine)
add_subdirectory(Launch)
add_subdirectory(Renderer)
