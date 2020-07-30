#include "Components/SphereComponent.hpp"
#include "Actors/Actor.hpp"
#include "World.hpp"

namespace oeng::engine
{
	SphereComponent::SphereComponent(AActor& owner, int update_order)
		:SceneComponent{owner, update_order}
	{
	}

	SphereComponent::~SphereComponent()
	{
		if (HasBegunPlay()) GetWorld().UnregisterCollision(*this);
	}

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
		if (!(IsActive() && other.IsActive())) return false;
		
		const auto dist_sqr = GetWorldPos().DistSqr(other.GetWorldPos());
		const auto r = radius_ + other.radius_;
		return dist_sqr < r*r;
	}

	void SphereComponent::OnBeginPlay()
	{
		GetWorld().RegisterCollision(*this);
	}
}
