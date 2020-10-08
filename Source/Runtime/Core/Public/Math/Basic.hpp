#pragma once
#include <concepts>
#include <numbers>
#include <random>

#ifndef OENG_DEFAULT_FLOAT
#define OENG_DEFAULT_FLOAT float
#endif

namespace oeng
{
inline namespace core
{
inline namespace math
{
using Float = OENG_DEFAULT_FLOAT;
static_assert(std::floating_point<Float>);

template <class T>
concept Arithmetic = std::is_arithmetic_v<T>;

template <class T, size_t L>
struct Vector;

template <std::floating_point T, size_t L>
struct UnitVec;

using Vec2 = Vector<Float, 2>;
using Vec3 = Vector<Float, 3>;
using Vec4 = Vector<Float, 4>;

using Vec2i = Vector<int32_t, 2>;
using Vec2u = Vector<uint32_t, 2>;
using Vec3i = Vector<int32_t, 3>;
using Vec3u = Vector<uint32_t, 3>;
using Vec4i = Vector<int32_t, 4>;
using Vec4u = Vector<uint32_t, 4>;

using Vec2i16 = Vector<int16_t, 2>;
using Vec2u16 = Vector<uint16_t, 2>;
using Vec3i16 = Vector<int16_t, 3>;
using Vec3u16 = Vector<uint16_t, 3>;
using Vec4i16 = Vector<int16_t, 4>;
using Vec4u16 = Vector<uint16_t, 4>;

using UVec2 = UnitVec<Float, 2>;
using UVec3 = UnitVec<Float, 3>;
using UVec4 = UnitVec<Float, 4>;

template <class T, size_t Row, size_t Col = Row>
struct Matrix;

using Mat2 = Matrix<Float, 2>;
using Mat3 = Matrix<Float, 3>;
using Mat4 = Matrix<Float, 4>;
using Mat2x3 = Matrix<Float, 2, 3>;
using Mat2x4 = Matrix<Float, 2, 4>;
using Mat3x2 = Matrix<Float, 3, 2>;
using Mat3x4 = Matrix<Float, 3, 4>;
using Mat4x2 = Matrix<Float, 4, 2>;
using Mat4x3 = Matrix<Float, 4, 3>;

template <class T>
[[nodiscard]] constexpr auto ToFloat(T x) noexcept
{
    return static_cast<PromotedFloat<T>>(x);
}

constexpr Float operator""_f(unsigned long long f) noexcept
{
    return static_cast<Float>(f);
}

constexpr Float operator""_f(long double f) noexcept
{
    return static_cast<Float>(f);
}

template <class T>
constexpr auto kPiV = std::numbers::pi_v<T>;
constexpr auto kPi = kPiV<Float>;

template <class T>
constexpr auto kSmallNumV = static_cast<T>(1e-8);
constexpr auto kSmallNum = kSmallNumV<Float>;

inline thread_local std::default_random_engine random_engine{std::random_device{}()};

/**
 * Check if given integer is convertible to target type without loss.
 * @tparam To Target integral type.
 * @tparam From Source integral type.
 * @param from Source integer value.
 * @return `true` if it is safely convertible.
 */
template <std::integral To, std::integral From>
[[nodiscard]] constexpr bool IsSafelyConvertible(From from) noexcept
{
    if constexpr (sizeof(To) > sizeof(From))
    {
        return true;
    }
    else if constexpr (std::is_signed_v<To> == std::is_signed_v<From>)
    {
        if constexpr (sizeof(To) == sizeof(From))
        {
            return true;
        }
        else
        {
            return from <= static_cast<From>(std::numeric_limits<To>::max()) &&
                   from >= static_cast<From>(std::numeric_limits<To>::lowest());
        }
    }
    else if constexpr (std::is_signed_v<To>)
    {
        return from <= static_cast<From>(std::numeric_limits<To>::max());
    }
    else
    {
        return from >= 0;
    }
}

/**
 * Try to cast given integer to target type.
 * @tparam To Target integral type.
 * @tparam From Source integral type.
 * @param from Source integer value.
 * @return Converted value.
 * @throw std::domain_error If it is not safely convertible.
 */
template <std::integral To, std::integral From>
[[nodiscard]] constexpr To SafeCast(From from)
{
    if (IsSafelyConvertible<To>(from))
        return static_cast<To>(from);

    throw std::domain_error{"Cannot convert without loss"};
}

static_assert(IsSafelyConvertible<char>(10));
static_assert(!IsSafelyConvertible<char>(1000));
static_assert(IsSafelyConvertible<int64_t>(1000));
static_assert(IsSafelyConvertible<int64_t>(1000ull));
static_assert(IsSafelyConvertible<int16_t>(30000));
static_assert(IsSafelyConvertible<int16_t>(-30000));
static_assert(!IsSafelyConvertible<int16_t>(300000));
static_assert(!IsSafelyConvertible<int16_t>(-300000));
static_assert(!IsSafelyConvertible<int16_t>(60000u));
static_assert(IsSafelyConvertible<int16_t>(20000u));
static_assert(!IsSafelyConvertible<uint16_t>(-20));
static_assert(IsSafelyConvertible<uint16_t>(2062));

template <std::integral T>
[[nodiscard]] constexpr T IntLog(T x, T base) noexcept
{
    T cnt = 0;
    while ((x /= base) > 0)
        ++cnt;
    return cnt;
}

template <std::integral T>
[[nodiscard]] constexpr T IntLogCeil(T x, T base) noexcept
{
    T cnt = 0;
    auto remain = false;

    for (T result{}; (result = x / base) > 0; ++cnt)
    {
        remain = remain || x % base;
        x = result;
    }

    return cnt + remain;
}

/**
 * @brief Returns random float in range [0, 1)
 */
[[nodiscard]] inline Float Rand() noexcept
{
    return std::uniform_real_distribution<Float>{0, 1}(random_engine);
}

/**
 * @brief Returns random int in range [min, max]
 */
template <std::integral T>
[[nodiscard]] T Rand(T min, T max) noexcept
{
    return std::uniform_int_distribution<T>{min, max}(random_engine);
}

/**
 * @brief Returns random float in range [min, max)
 */
template <std::floating_point T>
[[nodiscard]] T Rand(T min, T max) noexcept
{
    return std::uniform_real_distribution<T>{min, max}(random_engine);
}

template <std::floating_point T>
[[nodiscard]] T Gauss(T mean, T stddev) noexcept
{
    return std::normal_distribution<T>{mean, stddev}(random_engine);
}

template <class T>
[[nodiscard]] constexpr T Abs(T x) noexcept
{
    return x >= T(0) ? x : -x;
}

[[nodiscard]] Float Sqrt(Float x) noexcept
{
    return std::sqrt(x);
}

template <std::floating_point T>
[[nodiscard]] constexpr bool IsNearlyEqual(T a, T b, T tolerance = kSmallNumV<T>) noexcept
{
    return Abs(a - b) <= tolerance;
}

template <class T1, class T2, class T3 = std::common_type_t<T1, T2>>
[[nodiscard]] constexpr bool IsNearlyEqual(T1 a, T2 b, T3 tolerance = kSmallNumV<T3>) noexcept
{
    return IsNearlyEqual<PromotedFloat<T1, T2, T3>>(a, b, tolerance;
}

[[nodiscard]] constexpr bool IsNearlyZero(Float a, Float tolerance = kSmallNum) noexcept
{
    return Abs(a) <= tolerance;
}

[[nodiscard]] constexpr Float GetRangePct(Float min, Float max, Float val) noexcept
{
    return (val - min) / (max - min);
}

[[nodiscard]] constexpr auto MapRngClamp(Float in_min, Float in_max, Float out_min, Float out_max, Float val) noexcept
{
    return std::lerp(out_min, out_max, std::clamp(GetRangePct(in_min, in_max, val), 0_f, 1_f));
}

template <class T, class U, class V, class W, class X>
[[nodiscard]] constexpr auto MapRng(T in_min, U in_max, V out_min, W out_max, X val) noexcept
{
    return Lerp(out_min, out_max, GetRangePct(in_min, in_max, val));
}
} // namespace math
} // namespace core
} // namespace oeng
