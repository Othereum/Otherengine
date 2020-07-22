cmake_minimum_required(VERSION 3.14)
project(fmt-download NONE)

include(ExternalProject)
ExternalProject_Add(fmt-download
	GIT_REPOSITORY		https://github.com/fmtlib/fmt
	GIT_TAG				7.0.1
	GIT_SHALLOW			TRUE
	GIT_PROGRESS		TRUE
	SOURCE_DIR			"@CMAKE_CURRENT_BINARY_DIR@/fmt-src"
	BINARY_DIR			"@CMAKE_CURRENT_BINARY_DIR@/fmt-build"
	CONFIGURE_COMMAND	""
	BUILD_COMMAND		""
	INSTALL_COMMAND		""
	TEST_COMMAND		""
)
