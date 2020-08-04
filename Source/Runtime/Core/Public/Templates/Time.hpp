#pragma once
#include <chrono>

CORE_BEGIN

namespace time = std::chrono;
using namespace std::literals;
using Clock = time::steady_clock;
using Duration = Clock::duration;
using TimePoint = Clock::time_point;

CORE_END
