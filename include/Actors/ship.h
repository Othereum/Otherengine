#pragma once
#include "actor.h"

namespace game
{
	class ship : public actor
	{
	public:
		explicit ship(world& outer);

	private:
		bool is_moving_ = false;
	};
}
