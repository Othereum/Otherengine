#pragma once
#include "Actor.h"

namespace Game
{
	class ALaser : public AActor
	{
	public:
		explicit ALaser(CWorld& world);
		void UpdateActor(float deltaSeconds) override;
	};
}
