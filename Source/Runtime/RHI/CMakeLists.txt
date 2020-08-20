# Source/Runtime/RHI

add_module(RHI)

target_link_libraries(${PROJECT_NAME}-RHI
  PUBLIC
	${PROJECT_NAME}-Core
)

target_include_directories(${PROJECT_NAME}-RHI
  PRIVATE
    ${STB_INCLUDE_DIRS}
)
