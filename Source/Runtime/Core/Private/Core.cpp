#include "Core.hpp"
#include <thread>

namespace oeng
{
	OE_EXPORT bool engine_exist = false;

	static std::thread::id GetGameThreadId() noexcept
	{
		static const auto game_thread_id = std::this_thread::get_id();
		return game_thread_id;
	}

	OEAPI bool IsGameThread() noexcept
	{
		return std::this_thread::get_id() == GetGameThreadId();
	}

	OEAPI bool IsEngineExists() noexcept
	{
		return engine_exist;
	}
}
