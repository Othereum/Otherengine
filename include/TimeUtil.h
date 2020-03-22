#pragma once
#include <chrono>

namespace Game
{
	using namespace std::chrono;
	using TimePoint = steady_clock::time_point;
	using Duration = TimePoint::duration;
}
