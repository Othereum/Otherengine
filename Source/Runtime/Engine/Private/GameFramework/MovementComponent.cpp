#include "GameFramework/MovementComponent.hpp"
#include "Math.hpp"
#include "GameFramework/Actor.hpp"

namespace oeng
{
	MovementComponent::MovementComponent(AActor& owner, int update_order)
		:ActorComponent{owner, update_order}
	{
	}

	void MovementComponent::AddInput(const Vec3& input)
	{
		input_ += input;
	}

	void MovementComponent::OnUpdate(Float delta_seconds)
	{
		const auto lensqr = input_.LenSqr();
		if (IsNearlyZero(lensqr)) return;
		
		if (lensqr > 1) input_ /= sqrt(lensqr);
		GetOwner().SetPos(GetOwner().GetPos() + input_ * max_speed_);
		input_ = {};
	}
}
