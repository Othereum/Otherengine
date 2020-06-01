#include "Thread.hpp"
#include <thread>

namespace oeng
{
	static const std::thread::id kGameThreadId = std::this_thread::get_id();
	
	OEAPI bool IsGameThread() noexcept
	{
		return std::this_thread::get_id() == kGameThreadId;
	}
}
