#pragma once

enum e_no_init
{
	no_init
};

struct vector2
{
	float x, y;

	vector2() :x{}, y{} {}
	explicit vector2(e_no_init) {}
	vector2(const float x, const float y) :x{x}, y{y} {}
};
