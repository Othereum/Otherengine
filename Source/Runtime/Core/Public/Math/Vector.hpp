#pragma once
#include "Basic.hpp"
#include <cassert>
#include <functional>
#include <numeric>
#include <optional>

namespace otm
{
struct All_T
{
};

constexpr All_T All;

class DivByZero final : public std::logic_error
{
  public:
    DivByZero() : logic_error{"Division by zero"}
    {
    }
};

template <class T, size_t L>
struct VecBase
{
    T arr[L];

    template <class... Args>
    constexpr VecBase(Args... args) noexcept : arr{args...}
    {
    }
};

template <class T>
struct VecBase<T, 2>
{
    union {
        struct
        {
            T x, y;
        };
        T arr[2];
    };

    template <class... Args>
    constexpr VecBase(Args... args) noexcept : arr{args...}
    {
    }
};

template <class T>
struct VecBase<T, 3>
{
    union {
        struct
        {
            T x, y, z;
        };
        T arr[3];
    };

    template <class... Args>
    constexpr VecBase(Args... args) noexcept : arr{args...}
    {
    }
};

template <class T>
struct VecBase<T, 4>
{
    union {
        struct
        {
            T x, y, z, w;
        };
        T arr[4];
    };

    template <class... Args>
    constexpr VecBase(Args... args) noexcept : arr{args...}
    {
    }
};

template <class T, size_t L>
struct Vector : VecBase<T, L>
{
    using value_type = T;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = T&;
    using const_reference = const T&;
    using pointer = T*;
    using const_pointer = const T*;
    using iterator = T*;
    using const_iterator = const T*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    [[nodiscard]] static Vector Rand(const Vector& min, const Vector& max) noexcept
    {
        Vector v;
        std::transform(min.begin(), min.end(), max.begin(), v.begin(), [](T a, T b) { return otm::Rand(a, b); });
        return v;
    }

    [[nodiscard]] static Vector Rand(T min, T max) noexcept
    {
        Vector v;
        std::generate(v.begin(), v.end(), [&] { return otm::Rand(min, max); });
        return v;
    }

    constexpr Vector(All_T, T val) noexcept
    {
        std::fill(begin(), end(), val);
    }

    template <Arithmetic... Args>
    requires(sizeof...(Args) <= L) explicit(sizeof...(Args) == 1) constexpr Vector(Args... args) noexcept
        : VecBase<T, L>{static_cast<T>(args)...}
    {
    }

    template <class T2, size_t L2>
    explicit constexpr Vector(const Vector<T2, L2>& other) noexcept
    {
        std::transform(begin(), begin() + Min(L, L2), other.begin(), [](T val) { return static_cast<T2>(val); });
    }

    constexpr bool operator==(const Vector& r) const noexcept requires std::integral<T>
    {
        std::equal(begin(), end(), r.begin());
    }

    [[nodiscard]] constexpr T LenSqr() const noexcept
    {
        return *this | *this;
    }

    [[nodiscard]] auto Len() const noexcept
    {
        return Sqrt(LenSqr());
    }

    [[nodiscard]] constexpr T DistSqr(const Vector& v) const noexcept
    {
        return (*this - v).LenSqr();
    }

    [[nodiscard]] auto Dist(const Vector& v) const noexcept
    {
        return (*this - v).Len();
    }

    /**
     * @brief Normalize this vector
     * @throws DivByZero if IsNearlyZero(LenSqr())
     */
    void Normalize() requires std::floating_point<T>
    {
        if (!TryNormalize())
            throw DivByZero{};
    }

    bool TryNormalize() noexcept requires std::floating_point<T>
    {
        const auto lensqr = LenSqr();
        if (lensqr <= kSmallNumV<T>)
            return false;

        *this /= Sqrt(lensqr);
        return true;
    }

    /**
     * @brief Get normalized vector
     * @return Normalized vector or nullopt if length is zero
     */
    [[nodiscard]] std::optional<UnitVec<CommonFloat<T>, L>> Unit() const noexcept;

    [[nodiscard]] Matrix<T, 1, L>& AsRowMatrix() noexcept
    {
        return reinterpret_cast<Matrix<T, 1, L>&>(*this);
    }

    [[nodiscard]] const Matrix<T, 1, L>& AsRowMatrix() const noexcept
    {
        return reinterpret_cast<const Matrix<T, 1, L>&>(*this);
    }

    [[nodiscard]] Matrix<T, L, 1>& AsColMatrix() noexcept
    {
        return reinterpret_cast<Matrix<T, L, 1>&>(*this);
    }

    [[nodiscard]] const Matrix<T, L, 1>& AsColMatrix() const noexcept
    {
        return reinterpret_cast<const Matrix<T, L, 1>&>(*this);
    }

    [[nodiscard]] constexpr T* data() noexcept
    {
        return this->arr;
    }

    [[nodiscard]] constexpr const T* data() const noexcept
    {
        return this->arr;
    }

    [[nodiscard]] constexpr size_t size() const noexcept
    {
        return L;
    }

    constexpr T& operator[](size_t i) noexcept
    {
        assert(i < L);
        return this->arr[i];
    }

    constexpr const T& operator[](size_t i) const noexcept
    {
        assert(i < L);
        return this->arr[i];
    }

    [[nodiscard]] constexpr T& at(size_t i)
    {
        if (i >= L)
            OutOfRange();

        return this->arr[i];
    }

    [[nodiscard]] constexpr const T& at(size_t i) const
    {
        if (i >= L)
            OutOfRange();

        return this->arr[i];
    }

    constexpr void Negate() noexcept
    {
        std::transform(begin(), end(), begin(), std::negate<>{});
    }

    constexpr Vector operator-() const noexcept
    {
        auto v = *this;
        v.Negate();
        return v;
    }

    constexpr Vector& operator+=(const Vector& v) noexcept
    {
        std::transform(begin(), end(), v.begin(), begin(), std::plus<>{});
        return *this;
    }

    constexpr Vector& operator-=(const Vector<T2, L2>& v) noexcept
    {
        std::transform(begin(), end(), v.begin(), begin(), std::minus<>{});
        return *this;
    }

    constexpr Vector& operator*=(const Vector<T2, L2>& v) noexcept
    {
        std::transform(begin(), end(), v.begin(), begin(), std::multiplies<>{});
        return *this;
    }

    constexpr Vector& operator*=(T f) noexcept
    {
        std::transform(begin(), end(), begin(), [&](T x) { return x * f; });
        return *this;
    }

    constexpr Vector& operator/=(T f) noexcept
    {
        std::transform(begin(), end(), begin(), [&](T x) { return x / f; });
        return *this;
    }

    constexpr Vector operator+(const Vector& v) const noexcept
    {
        auto ret = *this;
        ret += v;
        return ret;
    }

    constexpr Vector operator-(const Vector& v) const noexcept
    {
        auto ret = *this;
        ret -= v;
        return ret;
    }

    constexpr Vector operator*(const Vector& v) const noexcept
    {
        auto ret = *this;
        ret *= v;
        return ret;
    }

    constexpr Vector operator*(T f) const noexcept
    {
        auto ret = *this;
        ret *= f;
        return ret;
    }

    constexpr Vector operator/(T f) const noexcept
    {
        auto ret = *this;
        ret /= f;
        return ret;
    }

    constexpr T operator|(const Vector& v) const noexcept
    {
        return std::inner_product(begin(), end(), v.begin(), T{});
    }

    [[nodiscard]] constexpr iterator begin() noexcept
    {
        return this->arr;
    }

    [[nodiscard]] constexpr const_iterator begin() const noexcept
    {
        return this->arr;
    }

    [[nodiscard]] constexpr const_iterator cbegin() const noexcept
    {
        return this->arr;
    }

    [[nodiscard]] constexpr iterator end() noexcept
    {
        return this->arr + L;
    }

    [[nodiscard]] constexpr const_iterator end() const noexcept
    {
        return this->arr + L;
    }

    [[nodiscard]] constexpr const_iterator cend() const noexcept
    {
        return this->arr + L;
    }

    [[nodiscard]] constexpr reverse_iterator rbegin() noexcept
    {
        return reverse_iterator{end()};
    }

    [[nodiscard]] constexpr const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator{end()};
    }

    [[nodiscard]] constexpr const_reverse_iterator crbegin() const noexcept
    {
        return const_reverse_iterator{cend()};
    }

    [[nodiscard]] constexpr reverse_iterator rend() noexcept
    {
        return reverse_iterator{begin()};
    }

    [[nodiscard]] constexpr const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator{begin()};
    }

    [[nodiscard]] constexpr const_reverse_iterator crend() const noexcept
    {
        return const_reverse_iterator{cbegin()};
    }

  private:
    [[noreturn]] static void OutOfRange()
    {
        throw std::out_of_range{"Vector out of range"};
    }
};

template <Arithmetic F, class T, size_t L>
constexpr auto operator*(F f, const Vector<T, L>& v) noexcept
{
    return v * f;
}

template <std::floating_point T, size_t L>
struct UnitVec
{
    [[nodiscard]] constexpr static UVec3 Forward() noexcept;
    [[nodiscard]] constexpr static UVec3 Right() noexcept;
    [[nodiscard]] constexpr static UVec3 Up() noexcept;

    [[nodiscard]] static UnitVec Rand() noexcept
    {
        Vector<T, L> v;
        v.Transform([](auto&&...) { return Gauss<T, T>(0, 1); });
        if (auto u = v.Unit())
            return *u;
        return Rand();
    }

    void RotateBy(const Quaternion<T>& q) noexcept
    {
        static_assert(L == 3);
        reinterpret_cast<Vector<T, 3>&>(*this).RotateBy(q);
    }

    [[nodiscard]] UnitVec RotatedBy(const Quaternion<T>& q) const noexcept
    {
        static_assert(L == 3);
        return static_cast<UnitVec>(reinterpret_cast<const Vector<T, 3>&>(*this).RotatedBy(q));
    }

    constexpr T operator[](size_t i) const noexcept
    {
        return v[i];
    }

    [[nodiscard]] constexpr const Vector<T, L>& Get() const noexcept
    {
        return v;
    }

    constexpr operator const Vector<T, L> &() const noexcept
    {
        return v;
    }

    constexpr const Vector<T, L>* operator->() const noexcept
    {
        return &v;
    }

    constexpr const Vector<T, L>& operator*() const noexcept
    {
        return v;
    }

  private:
    template <class, class>
    friend struct Angle;
    friend Vector<T, L>;
    friend detail::UnitVecBase<T, L>;

    template <class... Args>
    constexpr UnitVec(Args... args) noexcept : v{static_cast<T>(args)...}
    {
    }

    constexpr UnitVec(const Vector<T, L>& v) noexcept : v{v}
    {
    }

    Vector<T, L> v{};
};

template <class T, size_t L>
[[nodiscard]] constexpr T Min(const Vector<T, L>& v) noexcept
{
    auto m = v[0];
    for (size_t i = 1; i < L; ++i)
        m = Min(m, v[i]);
    return m;
}

template <class T, size_t L>
[[nodiscard]] constexpr T Max(const Vector<T, L>& v) noexcept
{
    auto m = v[0];
    for (size_t i = 1; i < L; ++i)
        m = Max(m, v[i]);
    return m;
}

template <class T, size_t L, class V = T>
[[nodiscard]] constexpr bool IsNearlyEqual(const Vector<T, L>& a, const Vector<T, L>& b,
                                           V tolerance = kSmallNumV<V>) noexcept
{
    for (size_t i = 0; i < L; ++i)
        if (!IsNearlyEqual(a[i], b[i], tolerance))
            return false;
    return true;
}

template <class T, size_t L, class V = T>
[[nodiscard]] constexpr bool IsNearlyZero(const Vector<T, L>& a, V tolerance = kSmallNumV<V>) noexcept
{
    for (auto x : a)
        if (!IsNearlyZero(x, tolerance))
            return false;
    return true;
}

template <size_t L, class T, class U, class V>
[[nodiscard]] constexpr auto Lerp(const Vector<T, L>& a, const Vector<U, L>& b, V alpha) noexcept
{
    return a + alpha * (b - a);
}

template <class T, class U>
constexpr auto operator^(const Vector<T, 3>& a, const Vector<U, 3>& b) noexcept
{
    return Vector{a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

template <class T>
constexpr Vector<T, 3>& operator^=(Vector<T, 3>& a, const Vector<T, 3>& b) noexcept
{
    return a = a ^ b;
}

template <class T, size_t L>
std::optional<UnitVec<CommonFloat<T>, L>> Vector<T, L>::Unit() const noexcept
{
    const auto lensqr = LenSqr();
    if (lensqr <= kSmallNumV<T>)
        return {};
    return UnitVec{*this / std::sqrt(ToFloat(lensqr))};
}

template <class Ratio, class T>
UnitVec<CommonFloat<T>, 2> Angle<Ratio, T>::ToVector() const noexcept
{
    return {{Cos(*this), Sin(*this)}};
}

template <class T, size_t L>
inline const Vector<T, L> Vector<T, L>::zero = Zero();

template <class T, size_t L>
inline const Vector<T, L> Vector<T, L>::one = One();

template <class T>
inline const Vector<T, 3> detail::VecBase<T, 3>::forward = Forward();

template <class T>
inline const Vector<T, 3> detail::VecBase<T, 3>::backward = Backward();

template <class T>
inline const Vector<T, 3> detail::VecBase<T, 3>::right = Right();

template <class T>
inline const Vector<T, 3> detail::VecBase<T, 3>::left = Left();

template <class T>
inline const Vector<T, 3> detail::VecBase<T, 3>::up = Up();

template <class T>
inline const Vector<T, 3> detail::VecBase<T, 3>::down = Down();

template <class T>
inline const UnitVec<T, 3> detail::UnitVecBase<T, 3>::forward = Forward();

template <class T>
inline const UnitVec<T, 3> detail::UnitVecBase<T, 3>::backward = Backward();

template <class T>
inline const UnitVec<T, 3> detail::UnitVecBase<T, 3>::right = Right();

template <class T>
inline const UnitVec<T, 3> detail::UnitVecBase<T, 3>::left = Left();

template <class T>
inline const UnitVec<T, 3> detail::UnitVecBase<T, 3>::up = Up();

template <class T>
inline const UnitVec<T, 3> detail::UnitVecBase<T, 3>::down = Down();
} // namespace otm
