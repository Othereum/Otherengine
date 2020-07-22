# Source/Runtime/Renderer

list(APPEND PUBLIC_LIBS
	${PROJECT_NAME}-Core
)

list(APPEND PRIVATE_LIBS
	$<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,${SDL2_LIBRARIES}>
	GLEW::glew
	soil2
)

add_engine_module(Renderer "${PUBLIC_LIBS}" "${PRIVATE_LIBS}")
target_include_directories(${PROJECT_NAME}-Renderer PRIVATE ${SDL2_INCLUDE_DIRS})
