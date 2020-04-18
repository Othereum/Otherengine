#pragma once
#include <chrono>
#include "Actor.h"

namespace oeng
{
	class ship : public AActor
	{
	public:
		explicit ship(CWorld& world);

	private:
		std::chrono::steady_clock::time_point nextAttack_;
		bool bIsMoving_ = false;
	};
}
