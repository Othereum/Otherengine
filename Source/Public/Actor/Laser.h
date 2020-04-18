#pragma once
#include "Actor.h"

namespace oeng
{
	class ALaser : public AActor
	{
	public:
		explicit ALaser(CWorld& world);
		void UpdateActor(float deltaSeconds) override;
	};
}
