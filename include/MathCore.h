#pragma once
#include <ratio>
#include <random>

namespace Game::Math
{
	using TPiRatio = std::ratio<5419351, 1725033>;
	constexpr auto kPi = static_cast<float>(TPiRatio::num) / TPiRatio::den;
	constexpr auto kSmallNumber = 1e-8f;
	constexpr auto kKindaSmallNumber = 1e-4f;

	template <class T, class U>
	constexpr auto Min(T a, U b) noexcept { return a < b ? a : b; }

	template <class T, class U>
	constexpr auto Max(T a, U b) noexcept { return a > b ? a : b; }

	template <class T, class U, class V>
	constexpr auto Clamp(T v, U min, V max) noexcept { return Math::Max(Math::Min(v, max), min); }

	template <class T, class U, class V = float>
	bool IsNearlyEqual(T a, U b, V tolerance = kSmallNumber) noexcept { return std::abs(a-b) < tolerance; }

	template <class T, class U = float>
	bool IsNearlyZero(T a, U tolerance = kSmallNumber) noexcept { return IsNearlyEqual(a, 0, tolerance); }

	
	inline thread_local std::default_random_engine gRandomEngine{std::random_device{}()};

	// [min, max] for int
	// [min, max) for float
	template <class T = float>
	[[nodiscard]] auto Rand(T min = 0, T max = std::is_integral_v<T> ? std::numeric_limits<T>::max() : 1) noexcept
	{
		if constexpr (std::is_integral_v<T>)
		{
			return std::uniform_int_distribution<T>{min, max}(gRandomEngine);
		}
		else
		{
			return std::uniform_real_distribution<T>{min, max}(gRandomEngine);
		}
	}
}
