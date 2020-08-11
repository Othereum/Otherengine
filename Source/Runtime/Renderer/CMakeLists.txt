# Source/Runtime/Renderer

add_module(Renderer)

target_link_libraries(${PROJECT_NAME}-Renderer
  PUBLIC
	${PROJECT_NAME}-Core

  PRIVATE
	$<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,${SDL2_LIBRARIES}>
	GLEW::glew
	soil2
)

target_include_directories(${PROJECT_NAME}-Renderer
  PRIVATE
	${SDL2_INCLUDE_DIRS}
)

target_compile_definitions(${PROJECT_NAME}-Renderer
  PRIVATE
    OE_OPENGL_VERSION_MAJOR=${OE_OPENGL_VERSION_MAJOR}
    OE_OPENGL_VERSION_MINOR=${OE_OPENGL_VERSION_MINOR}
)
