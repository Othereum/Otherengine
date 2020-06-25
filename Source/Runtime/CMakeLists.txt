# Source/Runtime

set(OE_LIB_TYPE SHARED CACHE STRING "SHARED | STATIC")
if(OE_LIB_TYPE STREQUAL "SHARED")
	set(OE_BUILD_STATIC 0)
elseif(OE_LIB_TYPE STREQUAL "STATIC")
	set(OE_BUILD_STATIC 1)
else()
	message(FATAL_ERROR "OE_LIB_TYPE should be [SHARED|STATIC]")
endif()

function(add_engine_module name public_libs private_libs)
	set(target ${PROJECT_NAME}-${name})
	file(GLOB_RECURSE sources "Private/*.cpp")
	add_library(${target} ${OE_LIB_TYPE} ${sources})
	target_link_libraries(${target} PUBLIC ${public_libs} PRIVATE ${private_libs})
	target_include_directories(${target} PUBLIC "Public")
	target_compile_definitions(${target} PRIVATE OE_BUILD)
	oe_setup_target(${target})
endfunction()

function(add_switch module name default description)
    set(${name} ${default} CACHE STRING ${description})
    target_compile_definitions(${PROJECT_NAME}-${module} PUBLIC ${name}=${${name}})
endfunction()

add_subdirectory(Core)
add_subdirectory(Engine)
add_subdirectory(Renderer)
