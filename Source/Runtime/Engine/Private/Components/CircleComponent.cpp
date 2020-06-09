#include "Components/CircleComponent.hpp"
#include "GameFramework/Actor.hpp"
#include "World.hpp"

namespace oeng
{
	CircleComponent::CircleComponent(AActor& owner, int update_order)
		:ActorComponent{owner, update_order}
	{
	}

	CircleComponent::~CircleComponent()
	{
		if (BegunPlay()) GetWorld().UnregisterCollision(*this);
	}

	void CircleComponent::TestOverlap(CircleComponent& other)
	{
		if (!(IsEnabled() && other.IsEnabled())) return;
		if (!(on_overlap_ || other.on_overlap_)) return;
		
		const auto distsqr = GetOwner().GetPos().DistSqr(other.GetOwner().GetPos());
		const auto r = radius_ + other.radius_;

		if (distsqr < r*r)
		{
			if (on_overlap_) on_overlap_(other);
			if (other.on_overlap_) other.on_overlap_(*this);
		}
	}

	void CircleComponent::BindOnOverlap(std::function<void(CircleComponent&)>&& on_overlap) noexcept
	{
		on_overlap_ = std::move(on_overlap);
	}
	
	void CircleComponent::OnBeginPlay()
	{
		GetWorld().RegisterCollision(*this);
	}
}
