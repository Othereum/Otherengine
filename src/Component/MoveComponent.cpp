#include "Component/MoveComponent.h"
#include "Actor/Actor.h"

namespace game
{
	CMoveComponent::CMoveComponent(AActor& owner, int updateOrder)
		:CActorComponent{owner, updateOrder}
	{
	}

	void CMoveComponent::Update(float deltaSeconds)
	{
		auto& owner = GetOwner();
		owner.SetPos(owner.GetPos() + vel_ * deltaSeconds);
		owner.SetRot(owner.GetRot() + rvel_ * deltaSeconds);
	}
}
