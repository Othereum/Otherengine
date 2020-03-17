#pragma once
#include "MathCore.h"

namespace Game
{
	template <class R>
	struct FRotation
	{
		static constexpr auto ratio = float(R::num) / R::den;

		constexpr FRotation() noexcept = default;
		constexpr explicit FRotation(float r) noexcept :r_{r} {}
		
		template <class S>
		constexpr FRotation(const FRotation<S>& r) noexcept { *this = r; }

		template <class S>
		constexpr FRotation& operator=(const FRotation<S>& r)& noexcept { r_ = r.Get() / r.ratio * ratio; return *this; }

		template <class S>
		constexpr FRotation& operator+=(const FRotation<S>& r)& noexcept { return *this = *this + r; }

		template <class S>
		constexpr FRotation operator+(const FRotation<S>& r) const noexcept { return *this + FRotation{r}; }
		constexpr FRotation operator+(const FRotation& r) const noexcept { return FRotation{r_ + r.Get()}; }

		constexpr FRotation operator*(const float f) const noexcept { return FRotation{r_ * f}; }
		constexpr FRotation operator/(const float f) const noexcept { return FRotation{r_ / f}; }

		constexpr FRotation operator-() const noexcept { return FRotation{-r_}; }

		template <class S>
		constexpr bool operator==(const FRotation<S>& r) const noexcept { return *this == FRotation{r}; }
		constexpr bool operator==(const FRotation& r) const noexcept { return r_ == r.r_; }

		template <class S>
		constexpr bool operator!=(const FRotation<S>& r) const noexcept { return !(*this == r); }

		template <class S>
		constexpr bool operator<(const FRotation<S>& r) const noexcept { return *this < FRotation{r}; }
		constexpr bool operator<(const FRotation& r) const noexcept { return r_ < r.r_; }

		template <class S>
		constexpr bool operator<=(const FRotation<S>& r) const noexcept { return !(*this > r); }

		template <class S>
		constexpr bool operator>(const FRotation<S>& r) const noexcept { return *this > FRotation{r}; }
		constexpr bool operator>(const FRotation& r) const noexcept { return r_ > r.r_; }

		template <class S>
		constexpr bool operator>=(const FRotation<S>& r) const noexcept { return !(*this < r); }

		[[nodiscard]] constexpr float Get() const noexcept { return r_; }

	private:
		float r_ = 0.f;
	};

	using TRadians = FRotation<std::ratio<Math::TPiRatio::num, Math::TPiRatio::den * 180>>;
	using TDegrees = FRotation<std::ratio<1>>;

	constexpr TRadians operator""_rad(unsigned long long f) noexcept { return TRadians{float(f)}; }
	constexpr TRadians operator""_rad(long double f) noexcept { return TRadians{float(f)}; }

	constexpr TDegrees operator""_deg(unsigned long long f) noexcept { return TDegrees{float(f)}; }
	constexpr TDegrees operator""_deg(long double f) noexcept { return TDegrees{float(f)}; }
	
	template <class R>
	FRotation<R> operator*(float f, const FRotation<R>& r) noexcept
	{
		return r * f;
	}
}
