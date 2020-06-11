#include "GameFramework/MovementComponent.hpp"
#include "GameFramework/Actor.hpp"
#include "Components/SceneComponent.hpp"

namespace oeng
{
	MovementComponent::MovementComponent(AActor& owner, int update_order)
		:ActorComponent{owner, update_order}
	{
	}

	void MovementComponent::OnUpdate(Float delta_seconds)
	{
		auto* root = GetOwner().GetRootComponent();
		CHECK_MSG(root, "Why do you use MoveComponent without RootComponent?");

		const auto moved = Move(*root, delta_seconds);
		const auto rotated = Rotate(*root);

		if (moved || rotated) root->RecalcWorldTrsf();
	}

	bool MovementComponent::Move(SceneComponent& root, Float delta_seconds) noexcept
	{
		const auto lensqr = mov_input_.LenSqr();
		if (lensqr <= kSmallNum) return false;
		
		if (lensqr > 1) mov_input_ /= sqrt(lensqr);
		root.rel_trsf_.pos += mov_input_ * (max_speed_ * delta_seconds);
		
		mov_input_ = {};
		return true;
	}

	bool MovementComponent::Rotate(SceneComponent& root) noexcept
	{
		if (IsNearlyEqual(rot_input_, Quat::identity)) return false;
		
		root.rel_trsf_.rot = rot_input_ * root.rel_trsf_.rot;
		rot_input_ = {};
		return true;
	}
}
