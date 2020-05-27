#include "Components/ActorComponent.hpp"
#include "Actor.hpp"

namespace oeng
{
	ActorComponent::ActorComponent(AActor& owner, const int update_order)
		:owner_{owner}, update_order_{update_order}
	{
	}

	Engine& ActorComponent::GetEngine() const noexcept
	{
		return GetOwner().GetEngine();
	}

	CWorld& ActorComponent::GetWorld() const noexcept
	{
		return GetOwner().GetWorld();
	}
}
