cmake_minimum_required(VERSION 3.12)
project(nlohmann_json-download NONE)

include(ExternalProject)
ExternalProject_Add(nlohmann_json-download
	GIT_REPOSITORY	https://github.com/nlohmann/json
	GIT_TAG			v${version}
	GIT_SHALLOW		TRUE
	GIT_PROGRESS	TRUE
	SOURCE_DIR		"${CMAKE_CURRENT_BINARY_DIR}/nlohmann_json-src"
	BINARY_DIR		"${CMAKE_CURRENT_BINARY_DIR}/nlohmann_json-build"
)
