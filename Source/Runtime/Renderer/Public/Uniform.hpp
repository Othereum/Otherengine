#pragma once
#include <variant>
#include "Math.hpp"

namespace oeng
{
	using Uniform = std::variant<Mat4, Vec4, Vec3, Vec2, float, int32_t>;
}
