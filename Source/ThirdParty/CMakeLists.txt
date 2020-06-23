# Sources/ThirdParty

add_subdirectory(otm)
add_subdirectory(omem)

file(GLOB_RECURSE SOIL2_SRC "SOIL2/src/SOIL2/*.c")
add_library(SOIL2 STATIC ${SOIL2_SRC})

set_target_properties(SOIL2 PROPERTIES POSITION_INDEPENDENT_CODE ON)
set(SOIL2_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SOIL2/src/SOIL2" PARENT_SCOPE)
