cmake_minimum_required(VERSION 3.14)

project("Otherengine"
	VERSION 0.1.0
	DESCRIPTION "3D Game Engine made by Othereum"
	HOMEPAGE_URL "https://github.com/Othereum/Otherengine"
)

include(CMake/Functions.cmake)
include(CMake/Options.cmake)

add_subdirectory(Source)

oe_install("Assets")
oe_install("Shaders")
oe_install("Config")

if(WIN32)
	install(DIRECTORY "../Binaries" DESTINATION "." FILES_MATCHING PATTERN "*.dll")
else()
	install(DIRECTORY "../Binaries" DESTINATION "." FILES_MATCHING PATTERN "*.so")
endif()
