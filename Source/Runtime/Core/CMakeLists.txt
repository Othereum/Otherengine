# Source/Runtime/Core

set(OE_USE_AVX2 FALSE)
add_module(Core)

oe_define_switch(Core OE_NAME_THREADSAFE)
oe_define_switch(Core OE_PATH_THREADSAFE)
oe_define_switch(Core OE_LOG_THREADSAFE)

oe_define_switch(Core OE_BUILD_DEBUG)
oe_define_switch(Core OE_BUILD_DEVELOPMENT)
oe_define_switch(Core OE_BUILD_TEST)
oe_define_switch(Core OE_BUILD_SHIPPING)

target_link_libraries(${PROJECT_NAME}-Core
  PUBLIC
	nlohmann_json::nlohmann_json
	fmt::fmt

  PRIVATE
	spdlog::spdlog
)

file(GLOB_RECURSE HEADER_FILES Public/*.hpp)
target_precompile_headers(${PROJECT_NAME}-Core PUBLIC ${HEADER_FILES})
