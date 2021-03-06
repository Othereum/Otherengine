#pragma once
#include <variant>

namespace oeng
{
inline namespace rhi
{
using ScalarParam = std::variant<Float, int32_t>;
using VectorParam = std::variant<Vec2, Vec3, Vec4, Vec2i, Vec3i, Vec4i>;
using MatrixParam = std::variant<Mat2, Mat3, Mat4, Mat2x3, Mat2x4, Mat3x2, Mat3x4, Mat4x2, Mat4x3>;
} // namespace rhi
} // namespace oeng

namespace std
{
RHI_API void from_json(const oeng::Json& json, oeng::ScalarParam& param);
RHI_API void from_json(const oeng::Json& json, oeng::VectorParam& param);
} // namespace std
