#pragma once
#include <chrono>

namespace oeng::core
{
	namespace time = std::chrono;
	using namespace std::literals;
	using Clock = time::steady_clock;
	using Duration = Clock::duration;
	using TimePoint = Clock::time_point;
}
