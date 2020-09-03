#include "Core.hpp"
#include <thread>

namespace oeng
{
inline namespace core
{
static const std::thread::id kGameThreadId = std::this_thread::get_id();

bool IsGameThread() noexcept
{
    return kGameThreadId == std::this_thread::get_id();
}
}
}
