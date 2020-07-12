#include "Stat.hpp"
#include <vector>

namespace oeng::detail
{
	OE_EXPORT std::vector<std::reference_wrapper<StopWatch>> timers;
	
	StopWatch::StopWatch(std::u8string_view name)
		:name_{name}
	{
		timers.emplace_back(*this);
	}
}
