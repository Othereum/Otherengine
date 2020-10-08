#pragma once
#include "Matrix.hpp"
#include "Quat.hpp"

namespace otm
{
struct Sphere
{
    Vec3 pos;
    Float radius;
};

constexpr bool IsOverlapped(const Sphere& a, const Sphere& b) noexcept
{
    const auto max_dist = a.radius + b.radius;
    return a.pos.DistSqr(b.pos) < max_dist * max_dist;
}

/**
 * \brief Make rotation matrix from quaternion
 * \tparam L Size of matrix to make
 */
template <size_t L = 3, class T = Float>
constexpr Matrix<T, L> MakeRotation(const Quaternion<T>& rotation) noexcept
{
    static_assert(L >= 3);

    const auto& [x, y, z] = rotation.v.data;
    const auto& w = rotation.s;

    Matrix<T, L> m;
    m[0] << 1 - 2 * (y * y + z * z) << 2 * (x * y + w * z) << 2 * (x * z - w * y);
    m[1] << 2 * (x * y - w * z) << 1 - 2 * (x * x + z * z) << 2 * (y * z + w * x);
    m[2] << 2 * (x * z + w * y) << 2 * (y * z - w * x) << 1 - 2 * (x * x + y * y);

    for (size_t i = 3; i < L; ++i)
        m[i][i] = 1;

    return m;
}

/**
 * \brief Make scale matrix
 * \tparam L Size of matrix to make
 */
template <size_t L = 3, class T = Float>
constexpr Matrix<T, L> MakeScale(const Vector<T, 3>& scale) noexcept
{
    static_assert(L >= 3);

    Matrix<T, L> s;
    s[0][0] = scale[0];
    s[1][1] = scale[1];
    s[2][2] = scale[2];

    for (size_t i = 3; i < L; ++i)
        s[i][i] = 1;

    return s;
}

/**
 * \brief Make translation matrix
 */
template <class T = Float>
constexpr Matrix<T, 4> MakeTranslation(const Vector<T, 3>& pos) noexcept
{
    return Matrix<T, 4>::Identity(pos.ToRowMatrix(), {0, 3});
}

/**
 * \brief Make simple projection matrix.
 * \tparam L Size of matrix to make. Must be 2 or greater.
 * \param screen Screen size. Must not be zero.
 */
template <size_t L = 2, class T = Float>
constexpr Matrix<CommonFloat<T>, L> MakeSimpleViewProj(const Vector<T, 2>& screen) noexcept
{
    static_assert(L >= 2);

    Matrix<CommonFloat<T>, L> proj;
    proj[0][0] = 2 / ToFloat(screen[0]);
    proj[1][1] = 2 / ToFloat(screen[1]);

    for (size_t i = 2; i < L; ++i)
        proj[i][i] = 1;

    return proj;
}

/**
 * \brief Make orthographic projection matrix.
 * \param screen Screen size. Must not be zero.
 * \param near Near plane distance.
 * \param far Far plane distance. Must be greater than near.
 */
template <class T = Float>
constexpr Matrix<T, 4> MakeOrtho(const Vector<T, 2>& screen, T near, T far) noexcept
{
    auto m = MakeSimpleViewProj<4>(screen);
    m[2][2] = 1 / (far - near);
    m[3][2] = near / (near - far);
    return m;
}

/**
 * \brief Make perspective projection matrix
 * \param screen Screen size. Must not be zero.
 * \param near Near plane distance.
 * \param far Far plane distance. Must be greater than near.
 * \param vfov Vertical viewing angle. Must be greater than zero.
 */
template <class Ratio, class T = Float>
constexpr Matrix<T, 4> MakePerspective(const Vector<T, 2>& screen, T near, T far, Angle<Ratio, T> vfov) noexcept
{
    const auto y_scale = 1 / Tan(vfov / 2);
    const auto x_scale = y_scale * (screen[1] / screen[0]);

    return {
        x_scale, 0, 0, 0,
        0, y_scale, 0, 0,
        0, 0, far / (far - near), 1,
        0, 0, (-near * far) / (far - near), 0
    };
}

/**
 * \brief Make look-at view matrix
 * \param eye Eye location
 * \param dir Look-at direction. Must never be parallel to up
 * \param up World up direction. Must never be parallel to dir
 * \return look-at view matrix or nullopt if dir and up are parallel to each other
 */
template <class T = Float>
static std::optional<Matrix<T, 4>> MakeLookAt(const Vector<T, 3>& eye,
                                              const UnitVec<T, 3>& dir, const UnitVec<T, 3>& up) noexcept
{
    try
    {
        auto i = *up ^ *dir;
        i.Normalize();
        auto j = *dir ^ i;
        j.Normalize();
        Vec3 t{eye | i, eye | j, eye | *dir};
        t.Negate();

        return Matrix<T, 4>{
            i[0], j[0], dir[0], 0,
            i[1], j[1], dir[1], 0,
            i[2], j[2], dir[2], 0,
            t[0], t[1], t[2], 1
        };
    }
    catch (...)
    {
        return std::nullopt;
    }
}
}
