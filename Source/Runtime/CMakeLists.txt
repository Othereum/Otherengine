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
	file(GLOB_RECURSE sources "Private/*.cpp")
	add_library(${PROJECT_NAME}-${name} ${OE_LIB_TYPE} ${sources})
	set_target_properties(${PROJECT_NAME}-${name} PROPERTIES CXX_STANDARD 20)
	target_link_libraries(${PROJECT_NAME}-${name} PUBLIC ${public_libs} PRIVATE ${private_libs})
	target_include_directories(${PROJECT_NAME}-${name} PUBLIC "Public")
	target_compile_definitions(${PROJECT_NAME}-${name} PRIVATE OE_BUILD)
endfunction()

function(add_switch module name default description)
    set(${name} ${default} CACHE STRING ${description})
    target_compile_definitions(${PROJECT_NAME}-${module} PUBLIC ${name}=${${name}})
endfunction()

add_subdirectory(Core)
add_subdirectory(Engine)
add_subdirectory(Renderer)
