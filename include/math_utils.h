#pragma once
#include "rotation.h"
#include "vector.h"

namespace game::math
{
	inline float cos(radians t) noexcept { return cosf(t.get()); }
	inline float sin(radians t) noexcept { return sinf(t.get()); }
	inline float tan(radians t) noexcept { return tanf(t.get()); }
	inline radians acos(float x) noexcept { return radians{acosf(x)}; }
	inline radians asin(float y) noexcept { return radians{asinf(y)}; }
	inline radians atan2(float y, float x) noexcept { return radians{atan2f(y, x)}; }

	template <class T>
	radians v2r(vector2<T> v) noexcept
	{
		return atan2(v.y, v.x);
	}

	inline fvector2 r2v(radians r) noexcept
	{
		return {cos(r), sin(r)};
	}

	inline radians rand_ang() noexcept
	{
		return radians{rand(-pi, pi)};
	}

	inline fvector2 rand_unit_vec() noexcept
	{
		return r2v(rand_ang());
	}
}
