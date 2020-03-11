#pragma once
#include <ratio>
#include <random>

namespace game::math
{
	using pi_ratio = std::ratio<5419351, 1725033>;
	constexpr auto pi = static_cast<float>(pi_ratio::num) / pi_ratio::den;
	constexpr auto small_number = 1e-8f;
	constexpr auto kinda_small_number = 1e-4f;

	template <class T, class U>
	constexpr auto min(T a, U b) noexcept { return a < b ? a : b; }

	template <class T, class U>
	constexpr auto max(T a, U b) noexcept { return a > b ? a : b; }

	template <class T, class U, class V>
	constexpr auto clamp(T v, U min, V max) noexcept { return math::max(math::min(v, max), min); }

	template <class T, class U, class V = float>
	bool is_nearly_equal(T a, U b, V tolerance = small_number) noexcept { return std::abs(a-b) < tolerance; }

	template <class T, class U = float>
	bool is_nearly_zero(T a, U tolerance = small_number) noexcept { return is_nearly_equal(a, 0, tolerance); }

	
	inline thread_local std::default_random_engine random_engine{std::random_device{}()};

	// [min, max] for int
	// [min, max) for float
	template <class T = float>
	[[nodiscard]] auto rand(T min = 0, T max = std::is_integral_v<T> ? std::numeric_limits<T>::max() : 1) noexcept
	{
		if constexpr (std::is_integral_v<T>)
		{
			return std::uniform_int_distribution<T>{min, max}(random_engine);
		}
		else
		{
			return std::uniform_real_distribution<T>{min, max}(random_engine);
		}
	}
}
