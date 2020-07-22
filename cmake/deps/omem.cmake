cmake_minimum_required(VERSION 3.14)
project(omem-download NONE)

include(ExternalProject)
ExternalProject_Add(omem-download
	GIT_REPOSITORY		https://github.com/Othereum/omem
	GIT_TAG				master
	GIT_SHALLOW			TRUE
	GIT_PROGRESS		TRUE
	SOURCE_DIR			"@CMAKE_CURRENT_BINARY_DIR@/omem-src"
	BINARY_DIR			"@CMAKE_CURRENT_BINARY_DIR@/omem-build"
	CONFIGURE_COMMAND	""
	BUILD_COMMAND		""
	INSTALL_COMMAND		""
	TEST_COMMAND		""
)
