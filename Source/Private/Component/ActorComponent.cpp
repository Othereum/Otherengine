#include "Components/ActorComponent.h"
#include "Actor.h"

namespace oeng
{
	CActorComponent::CActorComponent(AActor& owner, const int updateOrder)
		:owner_{owner}, updateOrder_{updateOrder}
	{
	}

	CEngine& CActorComponent::GetEngine() const noexcept
	{
		return GetOwner().GetEngine();
	}

	CWorld& CActorComponent::GetWorld() const noexcept
	{
		return GetOwner().GetWorld();
	}
}
