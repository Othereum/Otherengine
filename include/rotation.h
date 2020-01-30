#pragma once
#include <ratio>
#include <NEG.h>
#include <MathUtils.h>
NEG_BEGIN

template <class R>
struct rotation
{
	float r;
};

using radians = rotation<std::ratio<1>>;
using degrees = rotation<std::ratio<180, math::pi>>;

NEG_END