# Source/Runtime

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}/Binaries>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}/Binaries>)
set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)

string(REPLACE "/DNDEBUG" "" CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
string(REPLACE "/DNDEBUG" "" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
string(REPLACE "-DNDEBUG" "" CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
string(REPLACE "-DNDEBUG" "" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")

if(OE_USE_AVX2)
	add_compile_definitions(OE_USE_AVX2)
endif()


find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

oe_find_package(soil2)
oe_find_package(fmt 7)

set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "" FORCE)
oe_find_package(spdlog 1.7)
set_target_properties(spdlog::spdlog PROPERTIES POSITION_INDEPENDENT_CODE ON)

set(JSON_BuildTests OFF CACHE BOOL "" FORCE)
oe_find_package(nlohmann_json 3.8.0)

oe_download_package(omem)
oe_download_package(otm)


add_subdirectory(Core)
add_subdirectory(Engine)
add_subdirectory(Launch)
add_subdirectory(Renderer)
