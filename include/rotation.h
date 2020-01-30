#pragma once

#include <ratio>
#include "math_utils.h"

NEG_BEGIN

template <class R>
class rotation
{
public:
	static constexpr auto ratio = static_cast<float>(R::num) / R::den;

	constexpr rotation() noexcept :r_{} {}
	~rotation() noexcept = default;

	constexpr explicit rotation(const float r) noexcept :r_{r} {}
	constexpr explicit rotation(no_init_t) noexcept {}
	
	template <class S>
	constexpr rotation(const rotation<S>& r) noexcept { *this = r; }
	constexpr rotation(const rotation&) noexcept = default;

	template <class S>
	constexpr rotation(rotation<S>&& r) noexcept { *this = r; }
	constexpr rotation(rotation&&) noexcept = default;

	template <class S>
	constexpr rotation& operator=(const rotation<S>& r) & noexcept { r_ = r.get() / r.ratio * ratio; return *this; }
	constexpr rotation& operator=(const rotation&) & noexcept = default;

	template <class S>
	constexpr rotation& operator=(rotation<S>&& r) & noexcept { return *this = r; }
	constexpr rotation& operator=(rotation&&) & noexcept = default;

	template <class S>
	constexpr rotation operator+(const rotation<S>& r) const noexcept { return *this + rotation{r}; }
	constexpr rotation operator+(const rotation& r) const noexcept { return rotation{r_ + r.get()}; }

	constexpr rotation operator*(const float f) const noexcept { return rotation{r_ * f}; }
	constexpr rotation operator/(const float f) const noexcept { return rotation{r_ / f}; }

	[[nodiscard]] constexpr float get() const noexcept { return r_; }

private:
	float r_;
};

using radians = rotation<std::ratio<math::pi_ratio::num, math::pi_ratio::den * 180>>;
using degrees = rotation<std::ratio<1>>;

constexpr radians operator""_rad(const unsigned long long f) noexcept { return radians{static_cast<float>(f)}; }
constexpr radians operator""_rad(const long double f) noexcept { return radians{static_cast<float>(f)}; }

constexpr degrees operator""_deg(const unsigned long long f) noexcept { return degrees{static_cast<float>(f)}; }
constexpr degrees operator""_deg(const long double f) noexcept { return degrees{static_cast<float>(f)}; }

NEG_END

template <class R>
neg::rotation<R> operator*(const float f, const neg::rotation<R>& r) noexcept { return r * f; }
