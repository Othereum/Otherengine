# Source/Runtime

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}/Binaries>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}/Binaries>)

string(REPLACE "/DNDEBUG" "" CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
string(REPLACE "/DNDEBUG" "" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
string(REPLACE "-DNDEBUG" "" CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
string(REPLACE "-DNDEBUG" "" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")

set(OE_USE_AVX2 TRUE CACHE BOOL "Use AVX2?")
if(OE_USE_AVX2)
	add_compile_definitions(OE_USE_AVX2)
endif()

function(oe_setup_target target)
	get_target_property(target_type ${target} TYPE)

	if(OE_USE_AVX2)
		target_compile_options(${target} PRIVATE
			$<IF:$<CXX_COMPILER_ID:MSVC>,/arch:AVX2,-march=haswell>
		)
		target_compile_definitions(${target} PRIVATE OE_MODULE_USE_AVX2)
	endif()

	target_compile_options(${target} PRIVATE
		$<IF:$<CXX_COMPILER_ID:MSVC>,
			$<$<CONFIG:Release>:/GL> /MP /fp:fast /W4 /WX /wd4251 /wd4275,
			$<$<CONFIG:Release>:-flto> -ffast-math -Wall -Wextra -pedantic -Werror>
		$<$<CXX_COMPILER_ID:Clang>:-Wno-gnu-zero-variadic-macro-arguments>
	)

	target_link_options(${target} PRIVATE
		$<IF:$<CXX_COMPILER_ID:MSVC>,
			$<$<CONFIG:Release>:/LTCG>,
			$<$<CONFIG:Release>:-flto> -Wl$<COMMA>-rpath='$\{ORIGIN}'>
	)
endfunction()

add_subdirectory(Core)
add_subdirectory(Engine)
add_subdirectory(Launch)
add_subdirectory(Renderer)
