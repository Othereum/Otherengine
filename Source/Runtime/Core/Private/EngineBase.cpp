#include "EngineBase.hpp"

namespace oeng::core
{
	EngineBase* const kEngineBase = nullptr;

	static void RegisterEngine(EngineBase* engine) noexcept
	{
		assert(kEngineBase);
		const_cast<EngineBase*&>(kEngineBase) = engine;
	}

	EngineBase::EngineBase(std::u8string_view game_module_path)
		:thread_id_{(RegisterEngine(this), std::this_thread::get_id())},
		names_{String8{}}, paths_{fs::path{}}, game_dll_{String8{game_module_path}},
		game_name_{game_dll_.GetSymbol<const char8_t* const>(u8"kGameName"sv)}
	{
	}
}
