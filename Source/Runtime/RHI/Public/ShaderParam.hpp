#pragma once
#include <variant>

namespace oeng
{
inline namespace rhi
{
using ShaderParam = std::variant<
    Float, int32_t,

    Vec2, Vec3, Vec4,
    Vec2i, Vec3i, Vec4i,

    Mat2, Mat3, Mat4,
    Matrix<Float, 2, 3>, Matrix<Float, 2, 4>,
    Matrix<Float, 3, 2>, Matrix<Float, 3, 4>,
    Matrix<Float, 4, 2>, Matrix<Float, 4, 3>
>;

RHI_API void from_json(const Json& json, ShaderParam& param);
}
}
