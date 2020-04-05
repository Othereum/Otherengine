#pragma once
#include "Rotation.h"
#include "Algebra.h"

namespace game::math
{
	inline float Cos(Radians t) noexcept { return cosf(t.Get()); }
	inline float Sin(Radians t) noexcept { return sinf(t.Get()); }
	inline float Tan(Radians t) noexcept { return tanf(t.Get()); }
	inline Radians Acos(float x) noexcept { return Radians{acosf(x)}; }
	inline Radians Asin(float y) noexcept { return Radians{asinf(y)}; }
	inline Radians Atan2(float y, float x) noexcept { return Radians{atan2f(y, x)}; }

	inline Radians V2R(Vector2f v) noexcept
	{
		return Atan2(v.y(), v.x());
	}

	inline Vector2f R2V(Radians r) noexcept
	{
		return {Cos(r), Sin(r)};
	}

	inline Radians RandAng() noexcept
	{
		return Radians{Rand(-kPi, kPi)};
	}

	inline Vector2f RandUnitVec() noexcept
	{
		return R2V(RandAng());
	}

	inline Vector2f RandVec(const Vector2f& min, const Vector2f& max) noexcept
	{
		return {Rand(min.x(), max.x()), Rand(min.y(), max.y())};
	}

	inline float GetRangePct(const Vector2f& range, float val) noexcept
	{
		return GetRangePct(range.x(), range.y(), val);
	}

	inline float GetRangeValue(const Vector2f& range, float pct) noexcept
	{
		return Lerp(range.x(), range.y(), pct);
	}

	inline float MapRngClamp(const Vector2f& in_rng, const Vector2f& out_rng, float val) noexcept
	{
		const auto pct = Clamp(GetRangePct(in_rng, val), 0, 1);
		return GetRangeValue(out_rng, pct);
	}

	inline float MapRng(const Vector2f& in_rng, const Vector2f& out_rng, float val) noexcept
	{
		return GetRangeValue(out_rng, GetRangePct(in_rng, val));
	}

	inline Matrix4f CreateScale(const Vector3f& scale) noexcept
	{
		
	}
}
