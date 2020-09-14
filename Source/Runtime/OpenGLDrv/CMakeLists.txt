# Source/Runtime/OpenGLDrv

add_module(OpenGLDrv)

target_link_libraries(${PROJECT_NAME}-OpenGLDrv
  PUBLIC
	${PROJECT_NAME}-RHI

  PRIVATE
	GLEW::glew
	$<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,${SDL2_LIBRARIES}>
)

target_include_directories(${PROJECT_NAME}-OpenGLDrv
  PRIVATE
	${SDL2_INCLUDE_DIRS}
)

target_compile_definitions(${PROJECT_NAME}-OpenGLDrv
  PRIVATE
    OE_OPENGL_VERSION_MAJOR=${OE_OPENGL_VERSION_MAJOR}
    OE_OPENGL_VERSION_MINOR=${OE_OPENGL_VERSION_MINOR}
)
