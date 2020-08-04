
set(CMAKE_PREFIX_PATH "${PROJECT_SOURCE_DIR}/Source/ThirdParty/fmod/core;${PROJECT_SOURCE_DIR}/Source/ThirdParty/fmod/studio")

if(WIN32)
  set(CMAKE_FIND_LIBRARY_PREFIXES "")
  set(CMAKE_FIND_LIBRARY_SUFFIXES .lib .dll)
  set(CMAKE_LIBRARY_ARCHITECTURE x64)
  set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/core;C:/Program Files (x86)/FMOD SoundSystem/FMOD Studio API Windows/api/studio")
else()
  set(CMAKE_FIND_LIBRARY_PREFIXES lib)
  set(CMAKE_FIND_LIBRARY_SUFFIXES .so)
  set(CMAKE_LIBRARY_ARCHITECTURE x86_64)
endif()

find_library(fmod_core_lib NAMES fmod_vc fmod REQUIRED)
find_library(fmod_core_bin fmod REQUIRED)
find_path(fmod_core_inc fmod.hpp PATH_SUFFIXES inc REQUIRED)

find_library(fmod_studio_lib NAMES fmodstudio_vc fmodstudio REQUIRED)
find_library(fmod_studio_bin fmodstudio REQUIRED)
find_path(fmod_studio_inc fmod_studio.hpp PATH_SUFFIXES inc REQUIRED)
