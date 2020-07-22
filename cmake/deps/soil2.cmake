cmake_minimum_required(VERSION 3.14)
project(soil2-download NONE)

include(ExternalProject)
ExternalProject_Add(soil2-download
	GIT_REPOSITORY		https://github.com/SpartanJ/SOIL2
	GIT_TAG				release-1.20
	GIT_SHALLOW			TRUE
	GIT_PROGRESS		TRUE
	SOURCE_DIR			"@CMAKE_CURRENT_BINARY_DIR@/soil2-src"
	BINARY_DIR			"@CMAKE_CURRENT_BINARY_DIR@/soil2-build"
	CONFIGURE_COMMAND	""
	BUILD_COMMAND		""
	INSTALL_COMMAND		""
	TEST_COMMAND		""
)

configure_file(@PROJECT_SOURCE_DIR@/cmake/deps/soil2-build.cmake @CMAKE_CURRENT_BINARY_DIR@/soil2-src @ONLY)
