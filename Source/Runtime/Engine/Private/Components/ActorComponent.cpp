#include "Components/ActorComponent.hpp"
#include "Actors/Actor.hpp"

namespace oeng
{
	ActorComponent::ActorComponent(AActor& owner, const int update_order)
		:owner_{owner}, update_order_{update_order}
	{
	}

	void ActorComponent::BeginPlay()
	{
		OnBeginPlay();
		begun_play_ = true;
	}

	void ActorComponent::Update(Float delta_seconds)
	{
		OnUpdate(delta_seconds);
	}

	Engine& ActorComponent::GetEngine() const noexcept
	{
		return GetOwner().GetEngine();
	}

	World& ActorComponent::GetWorld() const noexcept
	{
		return GetOwner().GetWorld();
	}
}
