cmake_minimum_required(VERSION 3.14)
project(otm-download NONE)

include(ExternalProject)
ExternalProject_Add(otm-download
	GIT_REPOSITORY		https://github.com/Othereum/otm
	GIT_TAG				master
	GIT_SHALLOW			TRUE
	GIT_PROGRESS		TRUE
	SOURCE_DIR			"@CMAKE_CURRENT_BINARY_DIR@/otm-src"
	BINARY_DIR			"@CMAKE_CURRENT_BINARY_DIR@/otm-build"
	CONFIGURE_COMMAND	""
	BUILD_COMMAND		""
	INSTALL_COMMAND		""
	TEST_COMMAND		""
)
