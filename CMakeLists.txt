cmake_minimum_required(VERSION 3.8)
project("Otherengine")

file(GLOB_RECURSE SRC_FILES "Source/Private/*.cpp")
add_library(${PROJECT_NAME} STATIC ${SRC_FILES})

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 20
)

find_package(SDL2 2.0.12 REQUIRED)
find_package(GLEW 2.1.0-7 REQUIRED)
find_package(soil2 REQUIRED)
find_package(nlohmann_json 3.7.3 REQUIRED)
find_package(Microsoft.GSL REQUIRED)
add_subdirectory("Source/ThirdParty/otm")

target_include_directories(${PROJECT_NAME} PUBLIC "Source/Public" PRIVATE "Source/Private")
target_link_libraries(${PROJECT_NAME}
    PUBLIC otm nlohmann_json::nlohmann_json Microsoft.GSL::GSL
    PRIVATE SDL2::SDL2 GLEW::GLEW soil2
)

install(DIRECTORY "Shaders" DESTINATION "Engine")
