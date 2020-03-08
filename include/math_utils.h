#pragma once
#include "rotation.h"

namespace game::math
{
	constexpr auto pi = static_cast<float>(pi_ratio::num) / pi_ratio::den;
	constexpr auto small = 1e-8f;
	constexpr auto kinda_small = 1e-4f;

	template <class T, class U>
	constexpr auto min(T a, U b) noexcept { return a < b ? a : b; }

	template <class T, class U>
	constexpr auto max(T a, U b) noexcept { return a > b ? a : b; }

	template <class T, class U, class V>
	constexpr auto clamp(T v, U min, V max) noexcept { return math::max(math::min(v, max), min); }

	template <class T, class U, class V = float>
	bool is_nearly_equal(T a, U b, V tolerance = small) noexcept { return std::abs(a-b) < tolerance; }

	template <class T, class U = float>
	bool is_nearly_zero(T a, U tolerance = small) noexcept { return is_nearly_equal(a, 0, tolerance); }
}
