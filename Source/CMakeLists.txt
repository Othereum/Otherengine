# Source

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}/Binaries>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}/Binaries>)

find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)

oe_find_package(soil2)

set(BUILD_SHARED_LIBS ON CACHE BOOL "" FORCE)
oe_find_package(fmt 7)

set(SPDLOG_FMT_EXTERNAL ON CACHE BOOL "" FORCE)
oe_find_package(spdlog 1.7)
if(TARGET spdlog)
	set_target_properties(spdlog PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()

set(JSON_BuildTests OFF CACHE BOOL "" FORCE)
oe_find_package(nlohmann_json 3.8.0)

oe_download_package(omem)
oe_download_package(otm)

add_subdirectory(Runtime)
