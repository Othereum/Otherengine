cmake_minimum_required(VERSION 3.14)
project(soil2)

file(GLOB_RECURSE SOIL2_SRC "src/SOIL2/*.c")
add_library(soil2 STATIC ${SOIL2_SRC})
set_target_properties(soil2 PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_include_directories(soil2 INTERFACE src)
target_compile_options(soil2 PRIVATE $<IF:$<C_COMPILER_ID:MSVC>,/W0,-w>)

find_package(OpenGL REQUIRED)
target_link_libraries(soil2 PRIVATE ${OPENGL_LIBRARIES})
