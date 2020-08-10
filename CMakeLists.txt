cmake_minimum_required(VERSION 3.16)

project(Otherengine
	VERSION 0.2.0
	DESCRIPTION "3D Game Engine made by Othereum"
	HOMEPAGE_URL "https://github.com/Othereum/Otherengine"
)

message("${PROJECT_NAME} ${PROJECT_VERSION} made by Othereum")

set(CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	set(BUILD_GAME TRUE)
endif()

include(CMake/Version.cmake)
include(CMake/Functions.cmake)
include(CMake/Options.cmake)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}/Binaries>)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY $<1:${CMAKE_SOURCE_DIR}/Binaries>)

add_subdirectory(Source)

oe_install("Assets")
oe_install("Shaders")
oe_install("Config")

if(WIN32)
	install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} DESTINATION "." FILES_MATCHING PATTERN "*.dll")
else()
	install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} DESTINATION "." FILES_MATCHING PATTERN "*.so")
endif()
