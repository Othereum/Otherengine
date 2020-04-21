#pragma once
#include "Matrix.hpp"
#include "Rotation.h"
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
			return Scale() * RotationZ() * Translation();
		}

		[[nodiscard]] constexpr Mat4 Translation() const noexcept
		{
			auto t = Mat4::Identity();
			t[3] << pos.x << pos.y;
			return t;
		}

		[[nodiscard]] Mat4 RotationZ() const noexcept
		{
			const auto sin = math::Sin(rot), cos = math::Cos(rot);
			auto r = Mat4::Identity();
			r[0] << cos << sin;
			r[1] << -sin << cos;
			return r;
		}

		[[nodiscard]] constexpr Mat4 Scale() const noexcept
		{
			auto s = Mat4::Identity();
			s[0][0] = scale.x;
			s[1][1] = scale.y;
			return s;
		}
	};
}
