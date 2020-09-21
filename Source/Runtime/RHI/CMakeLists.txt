# Source/Runtime/RHI

add_module(RHI)

target_link_libraries(${PROJECT_NAME}-RHI
  PUBLIC
	${PROJECT_NAME}-Core

  PRIVATE
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,${SDL2_LIBRARIES}>
)

target_include_directories(${PROJECT_NAME}-RHI
  PRIVATE
    ${SDL2_INCLUDE_DIRS}
)
