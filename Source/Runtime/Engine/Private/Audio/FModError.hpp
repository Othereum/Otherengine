#pragma once
#include "fmod.hpp"

namespace oeng::engine
{
	std::u8string_view FModErrorString(FMOD_RESULT result);

	inline void FModCheck(FMOD_RESULT result)
	{
		if (result != FMOD_OK)
			Throw(FModErrorString(result));
	}

	template <class... Args>
	void FModCheck(FMOD_RESULT result, std::u8string_view message, const Args&... args)
	{
		if (result != FMOD_OK)
			Throw(u8"{}: {} ({})"sv, fmt::format(message, args...), FModErrorString(result), result);
	}
}
