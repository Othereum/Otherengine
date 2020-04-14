#pragma once
#include "Vector.h"
#include "Rotation.h"

namespace game
{
	struct Transform
	{
		FVector2 pos;
		Degrees rot;
		FVector2 scale;
	};
}
