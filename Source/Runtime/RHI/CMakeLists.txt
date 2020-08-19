# Source/Runtime/RHI

add_module(RHI)

target_link_libraries(${PROJECT_NAME}-RHI
  PUBLIC
	${PROJECT_NAME}-Core
)
