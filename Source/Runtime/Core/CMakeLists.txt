# Source/Runtime/Core

find_package(nlohmann_json 3.7.3 REQUIRED)
find_package(fmt 6.2.0 REQUIRED)
find_package(spdlog 1.4.2 REQUIRED)

list(APPEND LIBS otm omem nlohmann_json::nlohmann_json fmt::fmt spdlog::spdlog)
add_engine_module(Core "${LIBS}" "")
target_compile_definitions(${PROJECT_NAME}-Core INTERFACE OE_BUILD=0 PUBLIC OE_BUILD_STATIC=${OE_BUILD_STATIC})

add_switch(Core OE_NAME_THREADSAFE 0 "Should 'Name' class be thread-safe?")
add_switch(Core OE_PATH_THREADSAFE 0 "Should 'Path' class be thread-safe?")
add_switch(Core OE_SHARED_PTR_THREADSAFE 0 "Should 'SharedPtr' class be thread-safe?")
add_switch(Core OE_BUILD_MODE 0 "0 for debug, 1 for development, 2 for shipping")


