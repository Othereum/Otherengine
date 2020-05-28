cmake_minimum_required(VERSION 3.16)
project("Otherengine")

file(GLOB_RECURSE SRC_FILES "Source/Private/*.cpp" "Source/Private/*.hpp" "Source/Public/*.hpp")
add_library(${PROJECT_NAME} STATIC ${SRC_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)

function(add_switch name default description)
    set(${name} ${default} CACHE BOOL ${description})
    if(${name})
        target_compile_definitions(${PROJECT_NAME} PUBLIC ${name}=1)
    else()
        target_compile_definitions(${PROJECT_NAME} PUBLIC ${name}=0)
    endif()
endfunction()

add_switch(OENG_NAME_THREADSAFE FALSE "Should 'Name' class be thread-safe?")
add_switch(OENG_PATH_THREADSAFE FALSE "Should 'Path' class be thread-safe?")
add_switch(OENG_SHARED_PTR_THREADSAFE FALSE "Should 'SharedPtr' class be thread-safe as default?")

set(OENG_BUILD_MODE 0 CACHE STRING "0 for debug, 1 for development, 2 for shipping")
target_compile_definitions(${PROJECT_NAME} PUBLIC OENG_BUILD_MODE=${OENG_BUILD_MODE})

find_package(SDL2 2.0.12 REQUIRED)
find_package(GLEW 2.1.0-7 REQUIRED)
find_package(soil2 REQUIRED)
find_package(nlohmann_json 3.7.3 REQUIRED)
find_package(fmt 6.2.0 REQUIRED)
add_subdirectory("Source/ThirdParty/otm")

target_include_directories(${PROJECT_NAME} PUBLIC "Source/Public" PRIVATE "Source/Private")
target_link_libraries(${PROJECT_NAME}
    PUBLIC otm nlohmann_json::nlohmann_json fmt::fmt
    PRIVATE SDL2::SDL2 GLEW::GLEW soil2
)

set(OENG_BUILD_TESTS FALSE CACHE BOOL "Whether to build a test")
if(OENG_BUILD_TESTS)
	file(GLOB_RECURSE TEST_SRC_FILES "Source/Tests/*.cpp")
	add_executable("${PROJECT_NAME}Test" ${TEST_SRC_FILES})
	set_target_properties("${PROJECT_NAME}Test" PROPERTIES CXX_STANDARD 20)

	find_package(GTest 1.10.0 REQUIRED)
	target_link_libraries("${PROJECT_NAME}Test" PRIVATE ${PROJECT_NAME} GTest::GTest)

	enable_testing()
	add_test(NAME "${PROJECT_NAME} Test" COMMAND "${PROJECT_NAME}Test")
endif()

install(DIRECTORY "Assets" DESTINATION "Engine")
install(DIRECTORY "Shaders" DESTINATION "Engine")
