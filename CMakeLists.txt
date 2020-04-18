cmake_minimum_required(VERSION 3.8)
project("Otherengine")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Binaries)

file(GLOB_RECURSE SRC_FILES "Source/Private/*.cpp")
add_library(${PROJECT_NAME} SHARED ${SRC_FILES})

find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)
find_package(GLEW REQUIRED)
add_subdirectory("Source/ThirdParty/ola")

target_include_directories(${PROJECT_NAME} PUBLIC "Source/Public")
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2::SDL2_image GLEW::GLEW ola)
