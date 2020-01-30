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
	
	constexpr rotation(const float r) noexcept :r_{r} {}
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

	constexpr rotation& operator=(const float r) & noexcept { r_ = r; return *this; }
	[[nodiscard]] constexpr float get() const noexcept { return r_; }

private:
	float r_;
};

using radians = rotation<std::ratio<math::pi_ratio::num, math::pi_ratio::den * 180>>;
using degrees = rotation<std::ratio<1>>;

constexpr radians operator""_rad(const long double f) noexcept { return static_cast<float>(f); }
constexpr degrees operator""_deg(const long double f) noexcept { return static_cast<float>(f); }

template <class R>
constexpr rotation<R> operator+(const rotation<R>& r, float f) noexcept { return {r.get() + f}; }

template <class R>
constexpr rotation<R> operator+(float f, const rotation<R>& r) noexcept { return {r.get() + f}; }

NEG_END