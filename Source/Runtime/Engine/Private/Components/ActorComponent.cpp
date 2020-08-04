#include "Components/ActorComponent.hpp"
#include "Engine.hpp"
#include "Actors/Actor.hpp"

ENGINE_BEGIN

ActorComponent::ActorComponent(AActor& owner, const int update_order)
	:owner_{owner}, update_order_{update_order}
{
}

void ActorComponent::BeginPlay()
{
	OnBeginPlay();
	begun_play_ = true;
	if (auto_activate_) Activate();
}

void ActorComponent::Update(Float delta_seconds)
{
	OnUpdate(delta_seconds);
}

World& ActorComponent::GetWorld() const noexcept
{
	return owner_.GetWorld();
}

Renderer& ActorComponent::GetRenderer() noexcept
{
	return Engine::Get().GetRenderer();
}

ENGINE_END
