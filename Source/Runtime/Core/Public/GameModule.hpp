#pragma once
#include <string_view>
#include "Core.hpp"

#define GAME_API extern "C" OE_EXPORT
#define DEFINE_GAME_MODULE(game_name) GAME_API constinit const std::u8string_view kGameName = u8 ## #game_name ## sv

