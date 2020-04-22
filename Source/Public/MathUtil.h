#pragma once
#include "Rotation.h"
#include "Matrix.hpp"

namespace oeng::math
{
	inline float Cos(Radians t) noexcept { return cosf(t.Get()); }
	inline float Sin(Radians t) noexcept { return sinf(t.Get()); }
	inline float Tan(Radians t) noexcept { return tanf(t.Get()); }
	inline Radians Acos(float x) noexcept { return Radians{acosf(x)}; }
	inline Radians Asin(float y) noexcept { return Radians{asinf(y)}; }
	inline Radians Atan2(float y, float x) noexcept { return Radians{atan2f(y, x)}; }

	inline Radians V2R(const Vec2& v) noexcept
	{
		return Atan2(v.y, v.x);
	}

	inline Vec2 R2V(Radians r) noexcept
	{
		r = -r;
		return {Cos(r), Sin(r)};
	}

	inline Radians RandAng() noexcept
	{
		return Radians{Rand(-kPi, kPi)};
	}

	inline Vec2 RandUnitVec() noexcept
	{
		return R2V(RandAng());
	}

	inline Vec2 RandVec(const Vec2& min, const Vec2& max) noexcept
	{
		return {Rand(min.x, max.x), Rand(min.y, max.y)};
	}

	inline float GetRangePct(const Vec2& range, float val) noexcept
	{
		return GetRangePct(range.x, range.y, val);
	}

	inline float GetRangeValue(const Vec2& range, float pct) noexcept
	{
		return Lerp(range.x, range.y, pct);
	}

	inline float MapRngClamp(const Vec2& in_rng, const Vec2& out_rng, float val) noexcept
	{
		const auto pct = Clamp(GetRangePct(in_rng, val), 0, 1);
		return GetRangeValue(out_rng, pct);
	}

	inline float MapRng(const Vec2& in_rng, const Vec2& out_rng, float val) noexcept
	{
		return GetRangeValue(out_rng, GetRangePct(in_rng, val));
	}

	constexpr Mat4 Translation(const Vec2& pos) noexcept
	{
		auto t = Mat4::Identity();
		t[3] << pos.x << pos.y;
		return t;
	}

	inline Mat4 RotationZ(Radians rot) noexcept
	{
		rot = -rot;
		const auto sin = Sin(rot), cos = Cos(rot);
		auto r = Mat4::Identity();
		r[0] << cos << sin;
		r[1] << -sin << cos;
		return r;
	}

	constexpr Mat4 Scale(const Vec2& scale) noexcept
	{
		auto s = Mat4::Identity();
		s[0][0] = scale.x;
		s[1][1] = scale.y;
		return s;
	}
}
