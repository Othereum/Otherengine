cmake_minimum_required(VERSION 3.10)
project(@name@-download NONE)

include(ExternalProject)
ExternalProject_Add(@name@-download
	GIT_REPOSITORY		https://github.com/SpartanJ/SOIL2
	GIT_TAG				release-1.20
	GIT_SHALLOW			TRUE
	GIT_PROGRESS		TRUE
	SOURCE_DIR			"@src_dir@"
	BINARY_DIR			"@build_dir@"
	CONFIGURE_COMMAND	""
	BUILD_COMMAND		""
	INSTALL_COMMAND		""
	TEST_COMMAND		""
)

configure_file("@cmake_dir@/soil2-build.cmake" "@src_dir@/CMakeLists.txt" @ONLY)
