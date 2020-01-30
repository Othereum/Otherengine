#pragma once

#include "neg.h"

NEG_BEGIN

template <class T = float>
struct vector2
{
	T x, y;

	vector2() :x{}, y{} {}
	explicit vector2(no_init_t) {}
	vector2(T x, T y) :x{x}, y{y} {}

	template <class U>
	vector2(const vector2<U>& v) :x(v.x), y(v.y) {}

	template <class U>
	vector2(vector2<U>&& v) :x(v.x), y(v.y) {}

	void reset(T x, T y) { this->x = x; this->y = y; }

	template <class U>
	vector2& operator+=(const vector2<U>& a)
	{
		x += a.x;
		y += a.y;
		return *this;
	}

	template <class U>
	vector2<std::common_type_t<T, U>> operator*(U f) const
	{
		return {x*f, y*f};
	}

	template <class U>
	vector2<std::common_type_t<T, U>> operator-(const vector2<U>& v) const
	{
		return {x-v.x, y-v.y};
	}

	template <class U>
	vector2<std::common_type_t<T, U>> operator/(U f) const
	{
		return {x/f, y/f};
	}
};

NEG_END
