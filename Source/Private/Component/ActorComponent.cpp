#include "Components/ActorComponent.hpp"
#include "Actor.hpp"

namespace oeng
{
	CActorComponent::CActorComponent(AActor& owner, const int update_order)
		:owner_{owner}, update_order_{update_order}
	{
	}

	Engine& CActorComponent::GetEngine() const noexcept
	{
		return GetOwner().GetEngine();
	}

	CWorld& CActorComponent::GetWorld() const noexcept
	{
		return GetOwner().GetWorld();
	}
}
