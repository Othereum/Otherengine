# Source/Runtime

string(REPLACE "/DNDEBUG" "" CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
string(REPLACE "/DNDEBUG" "" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")
string(REPLACE "-DNDEBUG" "" CMAKE_C_FLAGS_RELWITHDEBINFO "${CMAKE_C_FLAGS_RELWITHDEBINFO}")
string(REPLACE "-DNDEBUG" "" CMAKE_CXX_FLAGS_RELWITHDEBINFO "${CMAKE_CXX_FLAGS_RELWITHDEBINFO}")

if(OE_USE_AVX2)
	add_compile_definitions(OE_USE_AVX2)
endif()

add_subdirectory(Core)
add_subdirectory(Engine)
add_subdirectory(Launch)
add_subdirectory(Renderer)
add_subdirectory(RenderCore)
add_subdirectory(RHI)
add_subdirectory(OpenGLDrv)
