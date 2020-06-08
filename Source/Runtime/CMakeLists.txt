# Source/Runtime

set(OE_BUILD_STATIC 0 CACHE STRING "0 for shared, 1 for static")
if(OE_BUILD_STATIC)
	set(OE_BUILD_MODE STATIC)
else()
	set(OE_BUILD_MODE SHARED)
endif()

add_library(${PROJECT_NAME} INTERFACE)

function(add_engine_module name public_libs private_libs)
	file(GLOB_RECURSE sources "Private/*.cpp")
	add_library(${PROJECT_NAME}-${name} ${OE_BUILD_MODE} ${sources})
	set_target_properties(${PROJECT_NAME}-${name} PROPERTIES CXX_STANDARD 20)
	target_link_libraries(${PROJECT_NAME}-${name} PUBLIC ${public_libs} PRIVATE ${private_libs})
	target_include_directories(${PROJECT_NAME}-${name} PUBLIC "Public")
	target_compile_definitions(${PROJECT_NAME}-${name} PRIVATE OE_BUILD=1)
	target_link_libraries(${PROJECT_NAME} INTERFACE ${PROJECT_NAME}-${name})
endfunction()

function(add_switch module name default description)
    set(${name} ${default} CACHE STRING ${description})
    target_compile_definitions(${PROJECT_NAME}-${module} PUBLIC ${name}=${${name}})
endfunction()

add_subdirectory(Core)
add_subdirectory(Engine)
add_subdirectory(GameFramework)
add_subdirectory(Renderer)
