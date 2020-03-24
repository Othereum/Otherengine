#pragma once
#include "MathCore.h"

namespace game
{
	template <class R>
	struct Rotation
	{
		static constexpr auto ratio = float(R::num) / R::den;

		constexpr Rotation() noexcept = default;
		constexpr explicit Rotation(float r) noexcept :r_{r} {}
		
		template <class S>
		constexpr Rotation(const Rotation<S>& r) noexcept { *this = r; }

		template <class S>
		constexpr Rotation& operator=(const Rotation<S>& r)& noexcept { r_ = r.Get() / r.ratio * ratio; return *this; }

		template <class S>
		constexpr Rotation& operator+=(const Rotation<S>& r)& noexcept { return *this = *this + r; }

		template <class S>
		constexpr Rotation operator+(const Rotation<S>& r) const noexcept { return *this + Rotation{r}; }
		constexpr Rotation operator+(const Rotation& r) const noexcept { return Rotation{r_ + r.Get()}; }

		constexpr Rotation operator*(const float f) const noexcept { return Rotation{r_ * f}; }
		constexpr Rotation operator/(const float f) const noexcept { return Rotation{r_ / f}; }

		constexpr Rotation operator-() const noexcept { return Rotation{-r_}; }

		template <class S>
		constexpr bool operator==(const Rotation<S>& r) const noexcept { return *this == Rotation{r}; }
		constexpr bool operator==(const Rotation& r) const noexcept { return r_ == r.r_; }

		template <class S>
		constexpr bool operator!=(const Rotation<S>& r) const noexcept { return !(*this == r); }

		template <class S>
		constexpr bool operator<(const Rotation<S>& r) const noexcept { return *this < Rotation{r}; }
		constexpr bool operator<(const Rotation& r) const noexcept { return r_ < r.r_; }

		template <class S>
		constexpr bool operator<=(const Rotation<S>& r) const noexcept { return !(*this > r); }

		template <class S>
		constexpr bool operator>(const Rotation<S>& r) const noexcept { return *this > Rotation{r}; }
		constexpr bool operator>(const Rotation& r) const noexcept { return r_ > r.r_; }

		template <class S>
		constexpr bool operator>=(const Rotation<S>& r) const noexcept { return !(*this < r); }

		[[nodiscard]] constexpr float Get() const noexcept { return r_; }

	private:
		float r_ = 0.f;
	};

	using Radians = Rotation<std::ratio<Math::TPiRatio::num, Math::TPiRatio::den * 180>>;
	using Degrees = Rotation<std::ratio<1>>;

	constexpr Radians operator""_rad(unsigned long long f) noexcept { return Radians{float(f)}; }
	constexpr Radians operator""_rad(long double f) noexcept { return Radians{float(f)}; }

	constexpr Degrees operator""_deg(unsigned long long f) noexcept { return Degrees{float(f)}; }
	constexpr Degrees operator""_deg(long double f) noexcept { return Degrees{float(f)}; }
	
	template <class R>
	Rotation<R> operator*(float f, const Rotation<R>& r) noexcept
	{
		return r * f;
	}
}
