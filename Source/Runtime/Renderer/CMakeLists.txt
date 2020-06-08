# Source/Runtime/Renderer

find_package(SDL2 2.0.12 REQUIRED)
find_package(GLEW 2.1.0-7 REQUIRED)
find_package(soil2 REQUIRED)

list(APPEND LIBS SDL2::SDL2 GLEW::GLEW soil2 ${PROJECT_NAME}-GameFramework)
add_engine_module(Renderer "" "${LIBS}")
