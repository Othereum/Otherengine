cmake_minimum_required(VERSION 3.14)
project(spdlog-download NONE)

include(ExternalProject)
ExternalProject_Add(spdlog-download
	GIT_REPOSITORY		https://github.com/gabime/spdlog
	GIT_TAG				v1.7.0
	GIT_SHALLOW			TRUE
	GIT_PROGRESS		TRUE
	SOURCE_DIR			"@CMAKE_CURRENT_BINARY_DIR@/spdlog-src"
	BINARY_DIR			"@CMAKE_CURRENT_BINARY_DIR@/spdlog-build"
	CONFIGURE_COMMAND	""
	BUILD_COMMAND		""
	INSTALL_COMMAND		""
	TEST_COMMAND		""
)
