#include "Core.hpp"
#include <thread>

namespace oeng
{
inline namespace core
{
static std::u8string_view game_name;
static bool is_game_thread_id_initialized = false;
static const std::thread::id kGameThreadId = [&]
{
    is_game_thread_id_initialized = true;
    return std::this_thread::get_id();
}();

void InitLogger();

// Called by EngineMain(), launch module
OE_EXPORT void SetGameName(std::u8string_view new_game_name)
{
    assert(game_name.empty());
    assert(!new_game_name.empty());
    game_name = new_game_name;
    InitLogger();
}

std::u8string_view GetGameName() noexcept
{
    assert(!game_name.empty());
    return game_name;
}

bool IsGameThread() noexcept
{
    // If not initialized yet, it is certain to be the main thread.
    if (!is_game_thread_id_initialized)
        return true;

    return kGameThreadId == std::this_thread::get_id();
}
}
}
