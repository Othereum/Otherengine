#pragma once
#include "MathUtil.h"

namespace oeng
{
	struct Transform
	{
		Vec2 pos;
		Degrees rot;
		Vec2 scale;

		explicit constexpr Transform(const Vec2& p = Vec2::Zero(), Degrees r = 0_deg, const Vec2& s = Vec2::One()) noexcept:
			pos{p}, rot{r}, scale{s}
		{
		}

		[[nodiscard]] constexpr Mat4 ToMatrix() const noexcept
		{
			return math::Scale(scale) * math::RotationZ(rot) * math::Translation(pos);
		}
	};
}
