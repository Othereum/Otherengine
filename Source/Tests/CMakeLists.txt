# Sources/Tests

set(OE_BUILD_TESTS FALSE CACHE BOOL "Whether to build a test")
if(OE_BUILD_TESTS)
	set(TEST_TARGET ${PROJECT_NAME}-Test)

	file(GLOB_RECURSE SOURCES "*.cpp")
	add_executable(${TEST_TARGET} ${SOURCES})
	set_target_properties(${TEST_TARGET} PROPERTIES CXX_STANDARD 20)

	find_package(GTest 1.10.0 REQUIRED)
	target_link_libraries(${TEST_TARGET} PRIVATE ${PROJECT_NAME}-Core GTest::GTest)

	enable_testing()
	add_test(NAME "${PROJECT_NAME} Test" COMMAND ${TEST_TARGET})
endif()

