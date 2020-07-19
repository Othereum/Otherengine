#include "EngineBase.hpp"

namespace oeng::core
{
	EngineBase* const kEngineBase = nullptr;

	EngineBase::EngineBase(std::u8string_view game_name)
		:thread_id_{(assert(!kEngineBase), const_cast<EngineBase*&>(kEngineBase) = this, std::this_thread::get_id())},
		game_name_{game_name},
		names_{String8{}}
	{
	}
}
