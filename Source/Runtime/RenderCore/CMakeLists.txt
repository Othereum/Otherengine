# Source/Runtime/RenderCore

add_module(RenderCore)

target_link_libraries(${PROJECT_NAME}-RenderCore
  PUBLIC
	${PROJECT_NAME}-Core

  PRIVATE
    ${PROJECT_NAME}-RHI
)

target_include_directories(${PROJECT_NAME}-RenderCore
  PRIVATE
    ${STB_INCLUDE_DIRS}
)
