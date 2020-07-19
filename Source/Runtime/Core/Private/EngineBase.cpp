#include "EngineBase.hpp"

namespace oeng::core
{
	EngineBase* const kEngineBase = nullptr;

	static void RegisterEngine(EngineBase* engine) noexcept
	{
		assert(kEngineBase);
		const_cast<EngineBase*&>(kEngineBase) = engine;
	}

	EngineBase::EngineBase(std::u8string_view game_name)
		:thread_id_{(RegisterEngine(this), std::this_thread::get_id())},
		game_name_{game_name}, names_{String8{}}, paths_{fs::path{}}
	{
	}
}
