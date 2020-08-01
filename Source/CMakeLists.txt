# Source

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
oe_find_package(nlohmann_json 3.9)


add_subdirectory(ThirdParty)
add_subdirectory(Runtime)
