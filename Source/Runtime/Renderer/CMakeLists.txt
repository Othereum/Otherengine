# Source/Runtime/Renderer

add_module(Renderer)

target_link_libraries(${PROJECT_NAME}-Renderer
  PUBLIC
	${PROJECT_NAME}-Engine

  PRIVATE
	$<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,${SDL2_LIBRARIES}>
)

target_include_directories(${PROJECT_NAME}-Renderer
  PRIVATE
	${SDL2_INCLUDE_DIRS}
)
