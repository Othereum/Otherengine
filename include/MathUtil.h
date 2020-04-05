#pragma once
#include "Rotation.h"

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

	inline FVector2 RandUnitVec() noexcept
	{
		return R2V(RandAng());
	}

	inline FVector2 RandVec(FVector2 min, FVector2 max) noexcept
	{
		return {Rand(min.x, max.x), Rand(min.y, max.y)};
	}

	inline float GetRangePct(FVector2 range, float val)
	{
		return GetRangePct(range.x, range.y, val);
	}

	inline float GetRangeValue(FVector2 range, float pct)
	{
		return Lerp(range.x, range.y, pct);
	}

	inline float MapRngClamp(FVector2 inRng, FVector2 outRng, float val)
	{
		auto pct = Clamp(GetRangePct(inRng, val), 0, 1);
		return GetRangeValue(outRng, pct);
	}

	inline float MapRng(FVector2 inRng, FVector2 outRng, float val)
	{
		return GetRangeValue(outRng, GetRangePct(inRng, val));
	}
}
