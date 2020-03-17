#pragma once
#include "Actor.h"

namespace Game
{
	class ship : public AActor
	{
	public:
		explicit ship(CEngine& app);

	private:
		bool bIsMoving_ = false;
	};
}
