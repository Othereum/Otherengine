#pragma once
#include "Vector.h"
#include "Rotation.h"

namespace game
{
	struct Transform
	{
		Vec2 pos;
		Degrees rot;
		Vec2 scale;
	};
}
