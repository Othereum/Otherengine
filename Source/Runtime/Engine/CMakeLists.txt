# Source/Runtime/Engine

add_module(Engine)

if(WIN32)
  set(CMAKE_FIND_LIBRARY_PREFIXES "")
  set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib" ".dll")
else()
  set(CMAKE_FIND_LIBRARY_PREFIXES "lib")
  set(CMAKE_FIND_LIBRARY_SUFFIXES ".a" ".so")
endif()

find_library(fmod_core_lib NAMES fmod_vc fmod
  HINTS "C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/core/lib/x64"
  REQUIRED
)

find_library(fmod_studio_lib NAMES fmodstudio_vc fmodstudio
  HINTS "C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/studio/lib/x64"
  REQUIRED
)

find_library(fmod_core_bin NAMES fmod
  HINTS "C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/core/lib/x64"
  REQUIRED
)

find_library(fmod_studio_bin NAMES fmodstudio
  HINTS "C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/studio/lib/x64"
  REQUIRED
)

find_path(fmod_core_inc NAMES fmod.hpp
  HINTS "C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/core/inc"
  REQUIRED
)

find_path(fmod_studio_inc NAMES fmod_studio.hpp
  HINTS "C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/studio/inc"
  REQUIRED
)

target_link_libraries(${PROJECT_NAME}-Engine
  PUBLIC
	${PROJECT_NAME}-Renderer

  PRIVATE
	$<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,${SDL2_LIBRARIES}>
	"${fmod_core_lib}"
	"${fmod_studio_lib}"
)

target_include_directories(${PROJECT_NAME}-Engine
  PRIVATE
	${SDL2_INCLUDE_DIRS}
	"${fmod_core_inc}"
	"${fmod_studio_inc}"
)

add_custom_command(TARGET ${PROJECT_NAME}-Engine POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${fmod_core_bin}"
    "${fmod_studio_bin}"
    $<TARGET_FILE_DIR:${PROJECT_NAME}-Engine>
)
