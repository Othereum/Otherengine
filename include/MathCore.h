#pragma once
#include <ratio>
#include <random>

namespace game::math
{
	using PiRatio = std::ratio<5419351, 1725033>;
	constexpr auto kPi = static_cast<float>(PiRatio::num) / PiRatio::den;
	constexpr auto kSmallNumber = 1e-8f;
	constexpr auto kKindaSmallNumber = 1e-4f;

	template <class T, class U>
	constexpr auto Min(T a, U b) noexcept { return a < b ? a : b; }

	template <class T, class U>
	constexpr auto Max(T a, U b) noexcept { return a > b ? a : b; }

	template <class T, class U, class V>
	constexpr auto Clamp(T v, U min, V max) noexcept { return math::Max(math::Min(v, max), min); }

	template <class T, class U, class V = float>
	bool IsNearlyEqual(T a, U b, V tolerance = kSmallNumber) noexcept { return std::abs(a-b) < tolerance; }

	template <class T, class U = float>
	bool IsNearlyZero(T a, U tolerance = kSmallNumber) noexcept { return IsNearlyEqual(a, 0, tolerance); }

	template <class T, class U, class V>
	float GetRangePct(T min, U max, V val) noexcept
	{
		const auto divisor = float(max - min);
		if (IsNearlyZero(divisor))
		{
			return (val >= max) ? 1.f : 0.f;
		}

		return float(val - min) / divisor;
	}
	
	template <class T, class U, class V> 
	auto Lerp(T a, U b, V alpha)
	{
		return a + alpha * (b - a);
	}
	
	inline thread_local std::default_random_engine random_engine{std::random_device{}()};

	// [min, max] for int
	// [min, max) for float
	template <class T = float>
	[[nodiscard]] auto Rand(T min = 0, T max = std::is_integral_v<T> ? std::numeric_limits<T>::max() : 1) noexcept
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
