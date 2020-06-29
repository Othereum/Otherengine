# Source/Runtime/Core

find_package(nlohmann_json 3.7 REQUIRED)
find_package(fmt 6.1 REQUIRED)
find_package(spdlog 1.4 REQUIRED)

list(APPEND PUBLIC_LIBS
	otm
	omem
	nlohmann_json::nlohmann_json
	fmt::fmt
	spdlog::spdlog
)

list(APPEND PRIVATE_LIBS
)

set(OE_USE_AVX2 FALSE)
add_engine_module(Core "${PUBLIC_LIBS}" "${PRIVATE_LIBS}")

add_switch(Core OE_NAME_THREADSAFE 0 "Should 'Name' class be thread-safe?")
add_switch(Core OE_PATH_THREADSAFE 0 "Should 'Path' class be thread-safe?")
add_switch(Core OE_SHARED_PTR_THREADSAFE 0 "Should 'SharedPtr' class be thread-safe?")
add_switch(Core OE_LOG_THREADSAFE 0 "Should logging be thread-safe?")
