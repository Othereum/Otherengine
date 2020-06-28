cmake_minimum_required(VERSION 3.14)

project("Otherengine"
	VERSION 0.1.0
	DESCRIPTION "3D Game Engine made by Othereum"
	HOMEPAGE_URL "https://github.com/Othereum/Otherengine"
	LANGUAGES C CXX
)

macro(set_global variable value)
	set(${variable} ${value})
	set(${variable} ${value} PARENT_SCOPE)
endmacro()

set_global(CMAKE_CXX_STANDARD 20)
set_global(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}/Binaries>)
set_global(CMAKE_LIBRARY_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}/Binaries>)

string(REPLACE "/DNDEBUG" "" CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
string(REPLACE "/DNDEBUG" "" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
string(REPLACE "-DNDEBUG" "" CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
string(REPLACE "-DNDEBUG" "" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
set_global(CMAKE_C_FLAGS_RELWITHDEBINFO ${CMAKE_C_FLAGS_RELWITHDEBINFO})
set_global(CMAKE_CXX_FLAGS_RELWITHDEBINFO ${CMAKE_CXX_FLAGS_RELWITHDEBINFO})

function(oe_target_setup target)
	target_compile_options(${target} PUBLIC
		$<IF:$<CXX_COMPILER_ID:MSVC>,
			$<$<CONFIG:Release>:/GL> /MP /fp:fast /arch:AVX2 /W4 /WX /wd4251,
			$<$<CONFIG:Release>:-flto> -ffast-math -march=haswell -Wall -Wextra -pedantic -Werror>)

	target_link_options(${target} PUBLIC
		$<IF:$<CXX_COMPILER_ID:MSVC>,
			$<$<CONFIG:Release>:/LTCG>,
			$<$<CONFIG:Release>:-flto> -Wl$<COMMA>-rpath='$\{ORIGIN}'>)
endfunction()

add_subdirectory(Source)

install(DIRECTORY "Assets" DESTINATION "Engine")
install(DIRECTORY "Shaders" DESTINATION "Engine")
install(DIRECTORY "Config" DESTINATION "Engine")
install(DIRECTORY "../Assets" DESTINATION ".")

if(WIN32)
	install(DIRECTORY "../Binaries" DESTINATION "." FILES_MATCHING PATTERN "*.dll")
else()
	install(DIRECTORY "../Binaries" DESTINATION "." FILES_MATCHING PATTERN "*.so")
endif()
