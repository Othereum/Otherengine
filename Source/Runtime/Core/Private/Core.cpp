#ifdef OE_MODULE_USE_AVX2
#error Core module should not use AVX2
#endif

#include "Core.hpp"
#include <cassert>
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
		assert(!game_name.empty());
		return game_name;
	}

	namespace log
	{
		void InitializeLogger();
	}
	
	OE_EXPORT void SetGameName(std::u8string_view name) noexcept
	{
		assert(game_name.empty());
		game_name = name;
		log::InitializeLogger();
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
