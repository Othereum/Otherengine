#include "Thread.hpp"
#include <thread>

namespace oeng
{
	static const auto kGameThreadId = std::this_thread::get_id();

	OEAPI bool IsGameThread() noexcept
	{
		return std::this_thread::get_id() == kGameThreadId;
	}
}
