# Source/Runtime/Engine

add_module(Engine)

target_link_libraries(${PROJECT_NAME}-Engine
  PUBLIC
    ${PROJECT_NAME}-RHI

  PRIVATE
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,${SDL2_LIBRARIES}>
    ${fmod_core_lib}
    ${fmod_studio_lib}
)

target_include_directories(${PROJECT_NAME}-Engine
  PRIVATE
    ${SDL2_INCLUDE_DIRS}
    ${fmod_core_inc}
    ${fmod_studio_inc}
    ${STB_INCLUDE_DIRS}
)

add_custom_command(TARGET ${PROJECT_NAME}-Engine POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${fmod_core_bin}
    ${fmod_studio_bin}
    $<TARGET_FILE_DIR:${PROJECT_NAME}-Engine>
)
