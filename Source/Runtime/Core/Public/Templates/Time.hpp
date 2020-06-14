#pragma once
#include <chrono>

namespace oeng
{
	namespace time = std::chrono;
	using namespace std::chrono_literals;
	using Clock = time::steady_clock;
	using Duration = Clock::duration;
	using TimePoint = Clock::time_point;
}
