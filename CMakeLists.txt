cmake_minimum_required(VERSION 3.8)
project("Otherengine")

file(GLOB_RECURSE SRC_FILES "Source/Private/*.cpp")
add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
)

find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)
find_package(GLEW REQUIRED)
add_subdirectory("Source/ThirdParty/ola")

target_include_directories(${PROJECT_NAME} PUBLIC "Source/Public")
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2::SDL2_image GLEW::GLEW ola)
