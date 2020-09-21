# Source/Runtime/Renderer

add_module(Renderer)
oe_link_sdl2(Renderer)

target_link_libraries(${PROJECT_NAME}-Renderer
  PUBLIC
	${PROJECT_NAME}-Engine
)
