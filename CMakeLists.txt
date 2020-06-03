cmake_minimum_required(VERSION 3.16)
project("Otherengine")

file(GLOB_RECURSE SRC_FILES "Source/Private/*.cpp")
add_library(${PROJECT_NAME} SHARED ${SRC_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)
target_include_directories(${PROJECT_NAME} PUBLIC "Source/Public" PRIVATE "Source/Private")
target_compile_definitions(${PROJECT_NAME} PRIVATE OE_BUILD=1 INTERFACE OE_BUILD=0 PUBLIC OE_BUILD_STATIC=0)

find_package(SDL2 2.0.12 REQUIRED)
find_package(GLEW 2.1.0-7 REQUIRED)
find_package(soil2 REQUIRED)
find_package(nlohmann_json 3.7.3 REQUIRED)
find_package(fmt 6.2.0 REQUIRED)
add_subdirectory("Source/ThirdParty/otm")
add_subdirectory("Source/ThirdParty/omem")

target_link_libraries(${PROJECT_NAME}
    PUBLIC otm omem nlohmann_json::nlohmann_json fmt::fmt
    PRIVATE SDL2::SDL2 GLEW::GLEW soil2
)

function(add_switch name default description)
    set(${name} ${default} CACHE STRING ${description})
    target_compile_definitions(${PROJECT_NAME} PUBLIC ${name}=${${name}})
endfunction()

add_switch(OE_NAME_THREADSAFE 0 "Should 'Name' class be thread-safe?")
add_switch(OE_PATH_THREADSAFE 0 "Should 'Path' class be thread-safe?")
add_switch(OE_SHARED_PTR_THREADSAFE 0 "Should 'SharedPtr' class be thread-safe?")
add_switch(OE_BUILD_MODE 0 "0 for debug, 1 for development, 2 for shipping")

set(OE_BUILD_TESTS FALSE CACHE BOOL "Whether to build a test")
if(OE_BUILD_TESTS)
	file(GLOB_RECURSE TEST_SRC_FILES "Source/Tests/*.cpp")
	add_executable("Test${PROJECT_NAME}" ${TEST_SRC_FILES})
	set_target_properties("Test${PROJECT_NAME}" PROPERTIES CXX_STANDARD 20)

	find_package(GTest 1.10.0 REQUIRED)
	target_link_libraries("Test${PROJECT_NAME}" PRIVATE ${PROJECT_NAME} GTest::GTest)

	enable_testing()
	add_test(NAME "Test ${PROJECT_NAME}" COMMAND "Test${PROJECT_NAME}")
endif()

install(DIRECTORY "Assets" DESTINATION "Engine")
install(DIRECTORY "Shaders" DESTINATION "Engine")
