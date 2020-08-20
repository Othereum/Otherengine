# Source/Runtime/RenderCore

add_module(RenderCore)

target_link_libraries(${PROJECT_NAME}-RenderCore
  PUBLIC
	${PROJECT_NAME}-Core

  PRIVATE
    ${PROJECT_NAME}-RHI
)
