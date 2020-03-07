#pragma once
#include <ratio>

namespace game::math
{
	using pi_ratio = std::ratio<5419351, 1725033>;
	constexpr auto pi = static_cast<float>(pi_ratio::num) / pi_ratio::den;
	constexpr auto small = 1e-8f;
	constexpr auto kinda_small = 1e-4f;

	template <class T, class U>
	constexpr auto min(T a, U b) noexcept { return a < b ? a : b; }

	template <class T, class U>
	constexpr auto max(T a, U b) noexcept { return a > b ? a : b; }

	template <class T, class U, class V>
	constexpr auto clamp(T v, U min, V max) noexcept { return math::max(math::min(v, max), min); }

	template <class T>
	constexpr T abs(T v) { return v < 0 ? -v : v; }

	template <class T, class U, class V>
	constexpr bool is_nearly_equal(T a, U b, V tolerance) { return abs(a-b) <= tolerance; }
}
