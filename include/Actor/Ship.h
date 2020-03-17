#pragma once
#include <chrono>
#include "Actor.h"

namespace Game
{
	class ship : public AActor
	{
	public:
		explicit ship(CEngine& engine);

	private:
		std::chrono::steady_clock::time_point nextAttack_;
		bool bIsMoving_ = false;
	};
}
