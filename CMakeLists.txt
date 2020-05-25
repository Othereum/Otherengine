cmake_minimum_required(VERSION 3.16)
project("Otherengine")

file(GLOB_RECURSE SRC_FILES "Source/Private/*.cpp" "Source/*.hpp")
add_library(${PROJECT_NAME} STATIC ${SRC_FILES})
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20)

set(OENG_NAME_THREADSAFE FALSE CACHE BOOL "Should 'Name' class be thread-safe?")
if(OENG_NAME_THREADSAFE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE OENG_NAME_THREADSAFE=1)
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE OENG_NAME_THREADSAFE=0)
endif()

find_package(SDL2 2.0.12 REQUIRED)
find_package(GLEW 2.1.0-7 REQUIRED)
find_package(soil2 REQUIRED)
find_package(nlohmann_json 3.7.3 REQUIRED)
find_package(Microsoft.GSL REQUIRED)
find_package(fmt 6.2.0 REQUIRED)
add_subdirectory("Source/ThirdParty/otm")

target_include_directories(${PROJECT_NAME} PUBLIC "Source/Public" PRIVATE "Source/Private")
target_link_libraries(${PROJECT_NAME}
    PUBLIC otm nlohmann_json::nlohmann_json Microsoft.GSL::GSL fmt::fmt
    PRIVATE SDL2::SDL2 GLEW::GLEW soil2
)

install(DIRECTORY "Assets" DESTINATION "Engine")
install(DIRECTORY "Shaders" DESTINATION "Engine")
