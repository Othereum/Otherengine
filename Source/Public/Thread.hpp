#pragma once
#include <thread>

namespace oeng
{
	static const std::thread::id kGameThreadId = std::this_thread::get_id();
	
	/**
	 * \brief Check if current context is in game(main) thread
	 */
	inline bool IsGameThread() noexcept
	{
		return std::this_thread::get_id() == kGameThreadId;
	}
}
