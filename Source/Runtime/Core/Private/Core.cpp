#include "Core.hpp"
#include <thread>

namespace oeng
{
inline namespace core
{
static std::u8string_view game_name;
static const std::thread::id kGameThreadId = std::this_thread::get_id();

void SetGameName(std::u8string_view game_name)
{
    assert(!game_name.empty());
    core::game_name = game_name;
}

std::u8string_view GetGameName() noexcept
{
    assert(!game_name.empty());
    return game_name;
}

bool IsGameThread() noexcept
{
    return kGameThreadId == std::this_thread::get_id();
}
}
}
