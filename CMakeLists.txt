cmake_minimum_required(VERSION 3.13.5)
project("Otherengine")

file(GLOB_RECURSE SRC_FILES "Source/Private/*.cpp")
add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
)

find_package(SDL2 CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(soil2 CONFIG REQUIRED)
add_subdirectory("Source/ThirdParty/otm")

target_include_directories(${PROJECT_NAME} PUBLIC "Source/Public" PRIVATE "Source/Private")
target_link_libraries(${PROJECT_NAME} PUBLIC otm PRIVATE SDL2::SDL2 GLEW::GLEW soil2)

install(DIRECTORY "Shaders" DESTINATION "Engine")
