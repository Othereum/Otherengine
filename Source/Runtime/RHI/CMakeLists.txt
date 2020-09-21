# Source/Runtime/RHI

add_module(RHI)
oe_link_sdl2(RHI)

target_link_libraries(${PROJECT_NAME}-RHI
  PUBLIC
	${PROJECT_NAME}-Core
)
