# Source/Runtime/Core

set(OE_USE_AVX2 FALSE)
add_module(Core)

add_switch(Core OE_NAME_THREADSAFE FALSE "Should 'Name' class be thread-safe?")
add_switch(Core OE_PATH_THREADSAFE FALSE "Should 'Path' class be thread-safe?")
add_switch(Core OE_LOG_THREADSAFE FALSE "Should logging be thread-safe?")

target_link_libraries(${PROJECT_NAME}-Core
  PUBLIC
	otm
	omem
	nlohmann_json::nlohmann_json
	fmt::fmt

  PRIVATE
	spdlog::spdlog
)
