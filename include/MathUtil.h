#pragma once
#include "Rotation.h"
#include "Vector.h"

namespace Game::Math
{
	inline float Cos(TRadians t) noexcept { return cosf(t.Get()); }
	inline float Sin(TRadians t) noexcept { return sinf(t.Get()); }
	inline float Tan(TRadians t) noexcept { return tanf(t.Get()); }
	inline TRadians Acos(float x) noexcept { return TRadians{acosf(x)}; }
	inline TRadians Asin(float y) noexcept { return TRadians{asinf(y)}; }
	inline TRadians Atan2(float y, float x) noexcept { return TRadians{atan2f(y, x)}; }

	template <class T>
	TRadians V2R(FVector2<T> v) noexcept
	{
		return Atan2(v.y, v.x);
	}

	inline TFVector2 R2V(TRadians r) noexcept
	{
		return {Cos(r), Sin(r)};
	}

	inline TRadians RandAng() noexcept
	{
		return TRadians{Rand(-kPi, kPi)};
	}

	inline TFVector2 RandUnitVec() noexcept
	{
		return R2V(RandAng());
	}

	inline TFVector2 RandVec(TFVector2 min, TFVector2 max) noexcept
	{
		return {Rand(min.x, max.x), Rand(min.y, max.y)};
	}
}
