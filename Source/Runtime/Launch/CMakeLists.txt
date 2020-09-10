# Source/Runtime/Game

add_executable(${CMAKE_PROJECT_NAME} "Launch.cpp")

if(BUILD_GAME)
	file(GLOB_RECURSE GAME_SRC "${CMAKE_SOURCE_DIR}/Source/${CMAKE_PROJECT_NAME}/Private/*.cpp")
	add_library(${CMAKE_PROJECT_NAME}-Module MODULE ${GAME_SRC})
	target_link_libraries(${CMAKE_PROJECT_NAME}-Module PRIVATE ${PROJECT_NAME}-Engine)
	target_include_directories(${CMAKE_PROJECT_NAME}-Module PRIVATE "${CMAKE_SOURCE_DIR}/Source/${CMAKE_PROJECT_NAME}/Public")
	target_compile_definitions(${CMAKE_PROJECT_NAME}-Module PRIVATE OE_BUILD)
	oe_setup_target(${CMAKE_PROJECT_NAME}-Module)

	add_dependencies(${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}-Module)
	target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
		OE_GAME_MODULE=\"$<TARGET_FILE_NAME:${CMAKE_PROJECT_NAME}-Module>\"
	)
else()
	target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE OE_GAME_MODULE=\"\")
endif()

target_link_libraries(${CMAKE_PROJECT_NAME} PRIVATE ${PROJECT_NAME}-Engine ${PROJECT_NAME}-Renderer)
target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE OE_BUILD)

set(OE_USE_AVX2 FALSE)
oe_setup_target(${CMAKE_PROJECT_NAME})

install(TARGETS ${CMAKE_PROJECT_NAME} DESTINATION "Binaries")
