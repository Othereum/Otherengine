cmake_minimum_required(VERSION 3.14)

project("Otherengine"
	VERSION 0.1.0
	DESCRIPTION "3D Game Engine made by Othereum"
	HOMEPAGE_URL "https://github.com/Othereum/Otherengine"
)

include(cmake/functions.cmake)
include(cmake/options.cmake)

add_subdirectory(Source)

oe_install("Assets")
oe_install("Shaders")
oe_install("Config")

if(WIN32)
	install(DIRECTORY "../Binaries" DESTINATION "." FILES_MATCHING PATTERN "*.dll")
else()
	install(DIRECTORY "../Binaries" DESTINATION "." FILES_MATCHING PATTERN "*.so")
endif()

option(OE_BUILD_DOCS "Whether to build documents with doxygen")
if(OE_BUILD_DOCS)
	add_custom_target(docs
		COMMAND Scripts/BuildDocs.$<IF:$<PLATFORM_ID:Windows>,bat,sh>
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
	)
endif()
