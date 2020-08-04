cmake_minimum_required(VERSION 3.14)
project(@name@-download NONE)

include(ExternalProject)
ExternalProject_Add(@name@-download
	GIT_REPOSITORY		https://github.com/nlohmann/json
	GIT_TAG				v3.9.0
	GIT_SHALLOW			TRUE
	GIT_PROGRESS		TRUE
	SOURCE_DIR			"@src_dir@"
	BINARY_DIR			"@build_dir@"
	CONFIGURE_COMMAND	""
	BUILD_COMMAND		""
	INSTALL_COMMAND		""
	TEST_COMMAND		""
)
