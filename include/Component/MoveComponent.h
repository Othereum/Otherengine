#pragma once
#include "ActorComponent.h"
#include "Rotation.h"
#include "Vector.h"

namespace game
{
	class CMoveComponent : public CActorComponent
	{
	public:
		explicit CMoveComponent(AActor& owner, int updateOrder = 10);

		void Update(float deltaSeconds) override;
		
		void SetVelocity(FVector2 newvel) noexcept { vel_ = newvel; }
		void SetAngularVelocity(Degrees newvel) noexcept { rvel_ = newvel; }

		[[nodiscard]] FVector2 GetVelocity() const noexcept { return vel_; }
		[[nodiscard]] Degrees GetAngularVelocity() const noexcept { return rvel_; }

	private:
		FVector2 vel_;
		Degrees rvel_;
	};
}
