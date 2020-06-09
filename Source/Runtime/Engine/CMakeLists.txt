# Source/Runtime/Engine

find_package(SDL2 2.0.12 REQUIRED)

list(APPEND PUBLIC_LIBS
	${PROJECT_NAME}-Core
	${PROJECT_NAME}-Renderer
)

list(APPEND PRIVATE_LIBS
	SDL2::SDL2
)

add_engine_module(Engine "${PUBLIC_LIBS}" "${PRIVATE_LIBS}")
