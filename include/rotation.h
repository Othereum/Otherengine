#pragma once
#include "math_core.h"

namespace game
{
	template <class R>
	class rotation
	{
	public:
		static constexpr auto ratio = float(R::num) / R::den;

		constexpr rotation() noexcept = default;
		constexpr explicit rotation(float r) noexcept :r_{r} {}
		
		template <class S>
		constexpr rotation(const rotation<S>& r) noexcept { *this = r; }

		template <class S>
		constexpr rotation& operator=(const rotation<S>& r)& noexcept { r_ = r.get() / r.ratio * ratio; return *this; }

		template <class S>
		constexpr rotation& operator+=(const rotation<S>& r)& noexcept { return *this = *this + r; }

		template <class S>
		constexpr rotation operator+(const rotation<S>& r) const noexcept { return *this + rotation{r}; }
		constexpr rotation operator+(const rotation& r) const noexcept { return rotation{r_ + r.get()}; }

		constexpr rotation operator*(const float f) const noexcept { return rotation{r_ * f}; }
		constexpr rotation operator/(const float f) const noexcept { return rotation{r_ / f}; }

		[[nodiscard]] constexpr float get() const noexcept { return r_; }

	private:
		float r_ = 0.f;
	};

	using radians = rotation<std::ratio<math::pi_ratio::num, math::pi_ratio::den * 180>>;
	using degrees = rotation<std::ratio<1>>;

	constexpr radians operator""_rad(unsigned long long f) noexcept { return radians{float(f)}; }
	constexpr radians operator""_rad(long double f) noexcept { return radians{float(f)}; }

	constexpr degrees operator""_deg(unsigned long long f) noexcept { return degrees{float(f)}; }
	constexpr degrees operator""_deg(long double f) noexcept { return degrees{float(f)}; }
}

template <class R>
game::rotation<R> operator*(float f, const game::rotation<R>& r) noexcept
{
	return r * f;
}
