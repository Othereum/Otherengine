# Source/ThirdParty

add_subdirectory(otm)

add_subdirectory(omem)
oe_target_setup(omem)

file(GLOB_RECURSE SOIL2_SRC "SOIL2/src/SOIL2/*.c")
add_library(SOIL2 STATIC ${SOIL2_SRC})
set_target_properties(SOIL2 PROPERTIES POSITION_INDEPENDENT_CODE ON)
target_include_directories(SOIL2 INTERFACE SOIL2/src)
oe_target_setup(SOIL2)
target_compile_options(SOIL2 PRIVATE $<IF:$<C_COMPILER_ID:MSVC>,/W0,-w>)
