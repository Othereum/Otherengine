cmake_minimum_required(VERSION 3.8)
project("NoEngineGame")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})

file(GLOB_RECURSE SRC_FILES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SRC_FILES})

find_package(Boost 1.72 REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)
find_package(GLEW REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(gsl-lite CONFIG REQUIRED)
find_package(Eigen3 CONFIG REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE "include" ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE SDL2::SDL2 SDL2::SDL2_image GLEW::GLEW fmt::fmt gsl::gsl-lite Eigen3::Eigen)

add_compile_definitions(PROJECT_NAME="${PROJECT_NAME}")
