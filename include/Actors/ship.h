#pragma once
#include "actor.h"

namespace game
{
	class ship : public actor
	{
	public:
		explicit ship(application& app);

	private:
		bool is_moving_ = false;
	};
}
