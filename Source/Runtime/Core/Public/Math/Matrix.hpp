#pragma once
#include "Vector.hpp"

namespace otm
{
namespace detail
{
template <class T, size_t R, size_t C>
struct MatrixBase
{
};

template <class T, size_t L>
struct MatrixBase<T, L, L>
{
    static const Matrix<T, L, L> identity;

    constexpr void Transpose() noexcept;

    // Matrix with ones on the main diagonal and zeros elsewhere
    static constexpr Matrix<T, L, L> Identity() noexcept;

    // Matrix that assigned other matrix to the identity matrix
    // Efficient version (a little :D)
    template <class T2, size_t L2>
    static constexpr Matrix<T, L, L> Identity(const Matrix<T2, L2, L2>& other) noexcept;

    // Matrix that assigned other matrix to the identity matrix with offset
    template <class T2, size_t R2, size_t C2>
    static constexpr Matrix<T, L, L> Identity(const Matrix<T2, R2, C2>& other,
                                              const Vector<ptrdiff_t, 2>& offset = {}) noexcept;
};
}

template <class T, size_t R, size_t C>
struct Matrix : detail::MatrixBase<T, R, C>
{
    using value_type = Vector<T, C>;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using reference = Vector<T, C>&;
    using const_reference = const Vector<T, C>&;
    using pointer = Vector<T, C>*;
    using const_pointer = const Vector<T, C>*;
    using iterator = Vector<T, C>*;
    using const_iterator = const Vector<T, C>*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    static const Matrix zero;

    [[nodiscard]] static constexpr Matrix Zero() noexcept
    {
        return {};
    }

    constexpr Matrix() noexcept = default;

    constexpr Matrix(All, T x) noexcept
    {
        for (auto& v : arr)
            for (auto& c : v)
                c = x;
    }

    template <class T2, size_t R2, size_t C2>
    explicit constexpr Matrix(const Matrix<T2, R2, C2>& other)
    {
        Assign(other);
    }

    template <class T2, size_t R2, size_t C2>
    constexpr Matrix(const Matrix<T2, R2, C2>& other, const Vector<ptrdiff_t, 2>& offset)
    {
        Assign(other, offset);
    }

    explicit constexpr Matrix(T x) noexcept
    {
        Assign({x});
    }

    template <class... Args>
    constexpr Matrix(T x, T y, Args ... args) noexcept
    {
        static_assert(sizeof...(Args) + 2 <= R * C, "Too many arguments");
        Assign({x, y, static_cast<T>(args)...});
    }

    template <class Fn, std::enable_if_t<std::is_invocable_r_v<T, Fn>, int>  = 0>
    explicit constexpr Matrix(Fn fn) noexcept(std::is_nothrow_invocable_v<Fn>)
    {
        for (auto& v : arr)
            v.Transform([&](auto&&...)
            {
                return fn();
            });
    }

    /**
     * \brief Assign elements of other matrix to this. The value of the unassigned elements does not change.
     * \note Does nothing if offset is out of range
     */
    template <class T2, size_t R2, size_t C2>
    constexpr void Assign(const Matrix<T2, R2, C2>& other, const Vector<ptrdiff_t, 2>& offset = {}) noexcept
    {
        if (offset[1] >= 0)
        {
            const auto size = Min(R - Min(R, static_cast<size_t>(offset[1])), R2);
            for (size_t i = 0; i < size; ++i)
                (*this)[i + offset[1]].Assign(other[i], offset[0]);
        }
        else
        {
            const auto size = Min(R, R2 - Min(R2, static_cast<size_t>(-offset[1])));
            for (size_t i = 0; i < size; ++i)
                (*this)[i].Assign(other[i - offset[1]], offset[0]);
        }
    }

    /**
     * \brief Assign elements from initializer list. The value of the unassigned elements does not change.
     */
    constexpr void Assign(std::initializer_list<T> list) noexcept
    {
        auto it = list.begin();
        for (size_t i = 0; i < R && it != list.end(); ++i)
            for (size_t j = 0; j < C && it != list.end(); ++j)
                arr[i][j] = *it++;
    }

    template <class T2>
    constexpr bool operator==(const Matrix<T2, R, C>& b) const noexcept
    {
        static_assert(!(std::is_floating_point_v<T> || std::is_floating_point_v<T2>),
            "Can't compare equality between floating point types. Use IsNearlyEqual() instead.");

        for (size_t i = 0; i < R; ++i)
            if (Row(i) != b[i])
                return false;

        return true;
    }

    template <class T2>
    constexpr bool operator!=(const Matrix<T2, R, C>& b) const noexcept
    {
        return !(*this == b);
    }

    template <class T2, size_t R2, size_t C2>
    constexpr bool operator==(const Matrix<T2, R2, C2>&) const noexcept
    {
        return false;
    }

    template <class T2, size_t R2, size_t C2>
    constexpr bool operator!=(const Matrix<T2, R2, C2>&) const noexcept
    {
        return true;
    }

    constexpr auto& operator[](size_t i) noexcept
    {
        assert(i < R);
        return arr[i];
    }

    constexpr auto& operator[](size_t i) const noexcept
    {
        assert(i < R);
        return arr[i];
    }

    [[nodiscard]] constexpr auto& Row(size_t i)
    {
        if (i >= R)
            OutOfRange();
        return arr[i];
    }

    [[nodiscard]] constexpr auto& Row(size_t i) const
    {
        if (i >= R)
            OutOfRange();
        return arr[i];
    }

    template <size_t L>
    [[nodiscard]] constexpr Vector<T, L> Row(size_t i) const
    {
        if (i >= R)
            OutOfRange();
        return Vector<T, L>{arr[i]};
    }

    template <size_t L = R>
    [[nodiscard]] constexpr auto Col(size_t c) const
    {
        if (c >= C)
            OutOfRange();
        Vector<T, L> v;
        for (size_t r = 0; r < Min(L, R); ++r)
            v[r] = arr[r][c];
        return v;
    }

    [[nodiscard]] constexpr auto& AsVectors() noexcept
    {
        return arr;
    }

    [[nodiscard]] constexpr auto& AsVectors() const noexcept
    {
        return arr;
    }

    [[nodiscard]] auto& AsFlatArr() noexcept
    {
        return reinterpret_cast<T(&)[R * C]>(arr);
    }

    [[nodiscard]] auto& AsFlatArr() const noexcept
    {
        return reinterpret_cast<const T(&)[R * C]>(arr);
    }

    constexpr Matrix operator+(const Matrix& b) const noexcept
    {
        auto c = *this;
        return c += b;
    }

    constexpr Matrix& operator+=(const Matrix& b) noexcept
    {
        for (auto i = 0; i < R; ++i)
            arr[i] += b[i];
        return *this;
    }

    constexpr Matrix operator-(const Matrix& b) const noexcept
    {
        auto c = *this;
        return c -= b;
    }

    constexpr Matrix& operator-=(const Matrix& b) noexcept
    {
        for (auto i = 0; i < R; ++i)
            arr[i] -= b[i];
        return *this;
    }

    constexpr Matrix operator*(T f) const noexcept
    {
        auto c = *this;
        return c *= f;
    }

    constexpr Matrix& operator*=(T f) noexcept
    {
        for (auto i = 0; i < R; ++i)
            arr[i] *= f;
        return *this;
    }

    constexpr Matrix operator/(T f) const noexcept
    {
        auto c = *this;
        return c /= f;
    }

    constexpr Matrix& operator/=(T f) noexcept
    {
        for (auto i = 0; i < R; ++i)
            arr[i] /= f;
        return *this;
    }

    template <class T2, size_t C2>
    constexpr Matrix<std::common_type_t<T, T2>, R, C2> operator*(const Matrix<T2, C, C2>& b) const noexcept
    {
        Matrix<std::common_type_t<T, T2>, R, C2> c;
        for (size_t i = 0; i < R; ++i)
            for (size_t j = 0; j < C2; ++j)
                c[i][j] = Row(i) | b.Col(j);
        return c;
    }

    constexpr Matrix& operator*=(const Matrix& b) noexcept
    {
        return *this = *this * b;
    }

    [[nodiscard]] constexpr Matrix<T, C, R> Transposed() const noexcept
    {
        Matrix<T, C, R> t;
        for (auto i = 0; i < R; ++i)
            for (auto j = 0; j < C; ++j)
                t[j][i] = arr[i][j];
        return t;
    }

    [[nodiscard]] constexpr T Det() const noexcept
    {
        static_assert(R == C);

        if constexpr (R == 1)
        {
            return arr[0][0];
        }
        else
        {
            auto plus = true;
            T det = 0;
            for (size_t i = 0; i < R; ++i)
            {
                const auto x = arr[i][R - 1] * Slice(i, R - 1).Det();
                if (plus)
                    det += x;
                else
                    det -= x;
                plus = !plus;
            }
            return det;
        }
    }

    [[nodiscard]] constexpr std::optional<Matrix> Inv() const noexcept
    {
        static_assert(R == C);
        static_assert(std::is_floating_point_v<T>);

        const auto det = Det();
        if (IsNearlyZero(det))
            return {};

        Matrix inv;
        for (size_t i = 0; i < R; ++i)
        {
            for (size_t j = 0; j < C; ++j)
            {
                inv[i][j] = Slice(j, i).Det() / det;
                if ((i + j) % 2 == 0)
                    inv[i][j] = -inv[i][j];
            }
        }
        return inv;
    }

    [[nodiscard]] constexpr Matrix<T, R - 1, C - 1> Slice(const size_t y, const size_t x) const noexcept
    {
        Matrix<T, R - 1, C - 1> m;
        size_t i = 0;
        for (; i < y; ++i)
        {
            size_t j = 0;
            for (; j < x; ++j)
                m[i][j] = arr[i][j];
            for (++j; j < C; ++j)
                m[i][j - 1] = arr[i][j];
        }
        for (++i; i < R; ++i)
        {
            size_t j = 0;
            for (; j < x; ++j)
                m[i - 1][j] = arr[i][j];
            for (++j; j < C; ++j)
                m[i - 1][j - 1] = arr[i][j];
        }
        return m;
    }

    [[nodiscard]] constexpr iterator begin() noexcept
    {
        return arr;
    }

    [[nodiscard]] constexpr const_iterator begin() const noexcept
    {
        return arr;
    }

    [[nodiscard]] constexpr const_iterator cbegin() const noexcept
    {
        return arr;
    }

    [[nodiscard]] constexpr iterator end() noexcept
    {
        return arr + R;
    }

    [[nodiscard]] constexpr const_iterator end() const noexcept
    {
        return arr + R;
    }

    [[nodiscard]] constexpr const_iterator cend() const noexcept
    {
        return arr + R;
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
        throw std::out_of_range{"Matrix out of range"};
    }

    Vector<T, C> arr[R];
};

template <class T, size_t R, size_t C, class V = T>
[[nodiscard]] constexpr bool IsNearlyEqual(const Matrix<T, R, C>& a, const Matrix<T, R, C>& b,
                                           V tolerance = kSmallNumV<V>) noexcept
{
    for (size_t i = 0; i < R; ++i)
        if (!IsNearlyEqual(a[i], b[i], tolerance))
            return false;
    return true;
}
template <class T, size_t R, size_t C, class V = T>
[[nodiscard]] constexpr bool IsNearlyZero(const Matrix<T, R, C>& a, V tolerance = kSmallNumV<V>) noexcept
{
    for (size_t i = 0; i < R; ++i)
        if (!IsNearlyZero(a[i], tolerance))
            return false;
    return true;
}

template <class T, size_t R, size_t C, class F, std::enable_if_t<std::is_convertible_v<F, T>, int> = 0>
constexpr auto operator*(F f, const Matrix<T, R, C>& m) noexcept
{
    return m * f;
}

template <class T, size_t R, size_t C>
std::ostream& operator<<(std::ostream& os, const Matrix<T, R, C>& m)
{
    os << m[0];
    for (size_t i = 1; i < R; ++i)
        os << '\n' << m[i];
    return os;
}

namespace detail
{
template <class T, size_t L>
constexpr void MatrixBase<T, L, L>::Transpose() noexcept
{
    auto& self = static_cast<Matrix<T, L, L>&>(*this);
    for (size_t i = 0; i < L; ++i)
        for (size_t j = 0; j < L; ++j)
        {
            using std::swap;
            swap(self[j][i], self[i][j]);
        }
}

template <class T, size_t L>
constexpr Matrix<T, L, L> MatrixBase<T, L, L>::Identity() noexcept
{
    Matrix<T, L, L> matrix;
    for (size_t i = 0; i < L; ++i)
        matrix[i][i] = 1;
    return matrix;
}

template <class T, size_t L>
template <class T2, size_t L2>
constexpr Matrix<T, L, L> MatrixBase<T, L, L>::Identity(const Matrix<T2, L2, L2>& other) noexcept
{
    Matrix<T, L, L> m{other};
    for (auto i = L2; i < L; ++i)
        m[i][i] = 1;
    return m;
}

template <class T, size_t L>
template <class T2, size_t R2, size_t C2>
constexpr Matrix<T, L, L> MatrixBase<T, L, L>::Identity(const Matrix<T2, R2, C2>& other,
                                                        const Vector<ptrdiff_t, 2>& offset) noexcept
{
    auto m = Identity();
    m.Assign(other, offset);
    return m;
}
}

template <class T, size_t L>
constexpr Matrix<T, 1, L> Vector<T, L>::ToRowMatrix() const noexcept
{
    Matrix<T, 1, L> m;
    for (size_t i = 0; i < L; ++i)
        m[0][i] = (*this)[i];
    return m;
}

template <class T, size_t L>
constexpr Matrix<T, L, 1> Vector<T, L>::ToColMatrix() const noexcept
{
    Matrix<T, L, 1> m;
    for (size_t i = 0; i < L; ++i)
        m[i][0] = (*this)[i];
    return m;
}

template <class T, size_t L>
inline const Matrix<T, L, L> detail::MatrixBase<T, L, L>::identity = Identity();

template <class T, size_t R, size_t C>
inline const Matrix<T, R, C> Matrix<T, R, C>::zero = Zero();
}
