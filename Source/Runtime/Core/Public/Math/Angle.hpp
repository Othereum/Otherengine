#pragma once
#include "Basic.hpp"
#include <ratio>

namespace otm
{
template <class Ratio>
struct Angle
{
    static constexpr auto ratio = static_cast<Float>(Ratio::num) / static_cast<Float>(Ratio::den);

    constexpr Angle() noexcept = default;

    constexpr explicit Angle(Float r) noexcept : val_{r}
    {
    }

    template <class R>
    constexpr Angle(const Angle<R>& r) noexcept : val_{r.get() / r.ratio * ratio}
    {
    }

    template <class R>
    constexpr Angle& operator=(const Angle<R>& r) noexcept
    {
        val_ = r.get() / r.ratio * ratio;
        return *this;
    }

    template <class R>
    constexpr Angle& operator+=(const Angle<R>& r) noexcept
    {
        return *this = *this + r;
    }

    template <class R>
    constexpr Angle operator+(const Angle<R>& r) const noexcept
    {
        return *this + Angle{r};
    }

    constexpr Angle operator+(const Angle& r) const noexcept
    {
        return Angle{val_ + r.get()};
    }

    template <class R>
    constexpr Angle& operator-=(const Angle<R>& r) noexcept
    {
        return *this = *this - r;
    }

    template <class R>
    constexpr Angle operator-(const Angle<R>& r) const noexcept
    {
        return *this - Angle{r};
    }

    constexpr Angle operator-(const Angle& r) const noexcept
    {
        return Angle{val_ - r.get()};
    }

    constexpr Angle& operator*=(Float f) noexcept
    {
        return *this = *this * f;
    }

    constexpr Angle operator*(Float f) const noexcept
    {
        return Angle{val_ * f};
    }

    constexpr Angle& operator/=(Float f) noexcept
    {
        return *this = *this / f;
    }

    constexpr Angle operator/(Float f) const noexcept
    {
        return Angle{val_ / f};
    }

    constexpr Angle operator-() const noexcept
    {
        return Angle{-val_};
    }

    constexpr bool operator<(const Angle& r) const noexcept
    {
        return val_ < r.val_;
    }

    constexpr bool operator>(const Angle& r) const noexcept
    {
        return val_ > r.val_;
    }

    constexpr bool operator<=(const Angle& r) const noexcept
    {
        return val_ <= r.val_;
    }

    constexpr bool operator>=(const Angle& r) const noexcept
    {
        return val_ >= r.val_;
    }

    template <class R>
    constexpr bool operator<(const Angle<R>& r) const noexcept
    {
        return *this < Angle{r};
    }

    template <class R>
    constexpr bool operator>(const Angle<R>& r) const noexcept
    {
        return *this > Angle{r};
    }

    template <class R>
    constexpr bool operator<=(const Angle<R>& r) const noexcept
    {
        return *this <= Angle{r};
    }

    template <class R>
    constexpr bool operator>=(const Angle<R>& r) const noexcept
    {
        return *this >= Angle{r};
    }

    [[nodiscard]] constexpr Float get() const noexcept
    {
        return val_;
    }

    [[nodiscard]] static Angle Rand() noexcept
    {
        return Rad{otm::Rand(-kPi, kPi)};
    }

  private:
    Float val_ = 0;
};

using Rad = Angle<std::ratio<66627445592888887, 21208174623389167 * 180>>;
using Deg = Angle<std::ratio<1>>;

constexpr Rad operator""_rad(unsigned long long f) noexcept
{
    return Rad{static_cast<Float>(f)};
}

constexpr Rad operator""_rad(long double f) noexcept
{
    return Rad{static_cast<Float>(f)};
}

constexpr Deg operator""_deg(unsigned long long f) noexcept
{
    return Deg{static_cast<Float>(f)};
}

constexpr Deg operator""_deg(long double f) noexcept
{
    return Deg{static_cast<Float>(f)};
}

template <Arithmetic T, class Ratio>
Rad operator*(T f, const Rad& r) noexcept
{
    return r * f;
}

[[nodiscard]] inline Float Cos(Rad t) noexcept
{
    return std::cos(t.get());
}

[[nodiscard]] inline Float Sin(Rad t) noexcept
{
    return std::sin(t.get());
}

[[nodiscard]] inline Float Tan(Rad t) noexcept
{
    return std::tan(t.get());
}

[[nodiscard]] inline Rad Acos(Float x) noexcept
{
    return Rad{std::acos(x)};
}

[[nodiscard]] inline Rad Asin(Float y) noexcept
{
    return Rad{std::asin(y)};
}

[[nodiscard]] inline Rad Atan(Float x) noexcept
{
    return Rad{std::atan(x)};
}

[[nodiscard]] inline Rad Atan2(Float y, Float x) noexcept
{
    return Rad{std::atan2(y, x)};
}
} // namespace otm
