#include "Components/SphereComponent.hpp"
#include "Engine/World.hpp"

namespace oeng
{
inline namespace engine
{

void SphereComponent::DoOverlap(SphereComponent& other)
{
    if (IsOverlap(other))
    {
        on_overlap_.Broadcast(other);
        other.on_overlap_.Broadcast(*this);
    }
}

bool SphereComponent::IsOverlap(const SphereComponent& other) const noexcept
{
    if (!IsActive() || !other.IsActive())
        return false;

    const auto dist_sqr = GetWorldPos().DistSqr(other.GetWorldPos());
    const auto r = GetScaledRadius() + other.GetScaledRadius();
    return dist_sqr < r * r;
}

void SphereComponent::OnBeginPlay()
{
    GetWorld().AddCollision(*this);
}

void SphereComponent::OnEndPlay()
{
    GetWorld().RemoveCollision(*this);
}

} // namespace engine
} // namespace oeng
