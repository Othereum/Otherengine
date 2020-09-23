#pragma once
#include "Core.hpp"
#include <string_view>

#define GAME_API extern "C" OE_EXPORT
#define DEFINE_GAME_MODULE(game_name) GAME_API const std::u8string_view kGameName = u8 ## #game_name ## sv

