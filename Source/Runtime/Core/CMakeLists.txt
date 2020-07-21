# Source/Runtime/Core

find_package(nlohmann_json 3.8 REQUIRED)
find_package(fmt 6.2 REQUIRED)
find_package(spdlog 1.4 REQUIRED)

list(APPEND PUBLIC_LIBS
	otm
	omem
	nlohmann_json::nlohmann_json
	fmt::fmt
)

list(APPEND PRIVATE_LIBS
	spdlog::spdlog
)

set(OE_USE_AVX2 FALSE)
add_engine_module(Core "${PUBLIC_LIBS}" "${PRIVATE_LIBS}")

add_switch(Core OE_NAME_THREADSAFE FALSE "Should 'Name' class be thread-safe?")
add_switch(Core OE_PATH_THREADSAFE FALSE "Should 'Path' class be thread-safe?")
add_switch(Core OE_LOG_THREADSAFE FALSE "Should logging be thread-safe?")
