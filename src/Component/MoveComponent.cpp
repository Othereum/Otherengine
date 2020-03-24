#include "Component/MoveComponent.h"
#include "Actor/Actor.h"

namespace game
{
	CMoveComponent::CMoveComponent(AActor& owner, int update_order)
		:CActorComponent{owner, update_order}
	{
	}

	void CMoveComponent::Update(float delta_seconds)
	{
		const auto acc = force_ / mass_;
		vel_ += acc * delta_seconds;
		force_ = {};
		
		auto& owner = GetOwner();
		owner.SetPos(owner.GetPos() + vel_ * delta_seconds);
		owner.SetRot(owner.GetRot() + rvel_ * delta_seconds);
	}
}
