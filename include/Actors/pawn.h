#pragma once
#include "actor.h"

namespace game
{
	class pawn : public actor
	{
	public:
		explicit pawn(world& outer);
	};
}
