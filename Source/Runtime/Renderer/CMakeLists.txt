# Source/Runtime/Renderer

find_package(SDL2 2.0.12 REQUIRED)
find_package(GLEW 2.1.0-7 REQUIRED)
find_package(soil2 REQUIRED)

list(APPEND PUBLIC_LIBS
	${PROJECT_NAME}-Core
)

list(APPEND PRIVATE_LIBS
	SDL2::SDL2
	GLEW::GLEW
	soil2
)

add_engine_module(Renderer "${PUBLIC_LIBS}" "${PRIVATE_LIBS}")
