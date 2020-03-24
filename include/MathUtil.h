#pragma once
#include "Rotation.h"
#include "Vector.h"

namespace game::Math
{
	inline float Cos(TRadians t) noexcept { return cosf(t.Get()); }
	inline float Sin(TRadians t) noexcept { return sinf(t.Get()); }
	inline float Tan(TRadians t) noexcept { return tanf(t.Get()); }
	inline TRadians Acos(float x) noexcept { return TRadians{acosf(x)}; }
	inline TRadians Asin(float y) noexcept { return TRadians{asinf(y)}; }
	inline TRadians Atan2(float y, float x) noexcept { return TRadians{atan2f(y, x)}; }

	template <class T>
	TRadians V2R(Vector2<T> v) noexcept
	{
		return Atan2(v.y, v.x);
	}

	inline FVector2 R2V(TRadians r) noexcept
	{
		return {Cos(r), Sin(r)};
	}

	inline TRadians RandAng() noexcept
	{
		return TRadians{Rand(-kPi, kPi)};
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
