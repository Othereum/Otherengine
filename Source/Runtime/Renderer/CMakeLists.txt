# Source/Runtime/Renderer

find_package(SDL2 REQUIRED)
find_package(GLEW 2.1.0 REQUIRED)

list(APPEND PUBLIC_LIBS
	${PROJECT_NAME}-Core
)

list(APPEND PRIVATE_LIBS
	${SDL2_LIBRARIES}
	GLEW::GLEW
	SOIL2
)

add_engine_module(Renderer "${PUBLIC_LIBS}" "${PRIVATE_LIBS}")
target_include_directories(${PROJECT_NAME}-Renderer PRIVATE ${SDL2_INCLUDE_DIRS} ${SOIL2_INCLUDE_DIR})
