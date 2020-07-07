#ifdef OE_MODULE_USE_AVX2
#error Core module should not use AVX2
#endif

#include "Core.hpp"
#include <thread>

namespace oeng
{
	static std::u8string_view game_name;
	OE_EXPORT bool engine_exist = false;

	static std::thread::id GetGameThreadId() noexcept
	{
		static const auto game_thread_id = std::this_thread::get_id();
		return game_thread_id;
	}

	std::u8string_view GetGameName() noexcept
	{
		return game_name;
	}

	OE_EXPORT void SetGameName(std::u8string_view name) noexcept
	{
		game_name = name;
	}

	bool IsGameThread() noexcept
	{
		return std::this_thread::get_id() == GetGameThreadId();
	}

	bool IsEngineExists() noexcept
	{
		return engine_exist;
	}
}
