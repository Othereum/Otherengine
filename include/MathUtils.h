#pragma once

#include "NEG.h"

#define MATH_BEGIN namespace math {
#define MATH_END }

NEG_BEGIN
MATH_BEGIN

template <class T>
constexpr T min(T a, T b) { return a < b ? a : b; }

template <class T>
constexpr T max(T a, T b) { return a > b ? a : b; }

template <class T>
constexpr T clamp(T v, T min, T max) { return math::max(math::min(v, max), min); }

template <class T>
constexpr T abs(T v) { return v < 0 ? -v : v; }

template <class T>
constexpr bool is_nearly_equal(T a, T b, T tolerance) { return abs(a-b) <= tolerance; }

MATH_END
NEG_END

#undef MATH_BEGIN
#undef MATH_END
