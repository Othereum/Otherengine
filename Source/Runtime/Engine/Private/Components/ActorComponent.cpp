#include "Components/ActorComponent.hpp"
#include "Engine.hpp"
#include "Actors/Actor.hpp"

namespace oeng
{
inline namespace engine
{
void ActorComponent::BeginPlay()
{
    OnBeginPlay();
    begun_play_ = true;

    if (auto_activate)
        Activate();
}

void ActorComponent::Update(Float delta_seconds)
{
    OnUpdate(delta_seconds);
}

void ActorComponent::Activate()
{
    if (!is_active_)
    {
        is_active_ = true;
        OnActivated();
    }
}

void ActorComponent::Deactivate()
{
    if (is_active_)
    {
        is_active_ = false;
        OnDeactivated();
    }
}

World& ActorComponent::GetWorld() const noexcept
{
    return owner_->GetWorld();
}
}
}
