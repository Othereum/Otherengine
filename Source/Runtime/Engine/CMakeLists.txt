# Source/Runtime/Engine

find_package(SDL2 2.0.12 REQUIRED)
add_engine_module(Engine ${PROJECT_NAME}-Core SDL2::SDL2)
