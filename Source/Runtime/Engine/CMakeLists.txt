# Source/Runtime/Engine

add_module(Engine)
oe_link_sdl2(Engine)

target_link_libraries(${PROJECT_NAME}-Engine
  PUBLIC
    ${PROJECT_NAME}-Core

  PRIVATE
    ${PROJECT_NAME}-RHI
    ${fmod_core_lib}
    ${fmod_studio_lib}
    stb
)

target_include_directories(${PROJECT_NAME}-Engine
  PRIVATE
    ${fmod_core_inc}
    ${fmod_studio_inc}
)

add_custom_command(TARGET ${PROJECT_NAME}-Engine POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${fmod_core_bin}
    ${fmod_studio_bin}
    $<TARGET_FILE_DIR:${PROJECT_NAME}-Engine>
)
