#pragma once
#include "Core.hpp"
#include <string_view>

#define DEFINE_GAME_MODULE(game_name) extern "C" OE_EXPORT const std::u8string_view kGameName = u8## #game_name##sv

