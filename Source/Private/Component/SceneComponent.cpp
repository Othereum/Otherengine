#include "Components/SceneComponent.hpp"
#include "Assert.hpp"

namespace oeng
{
	SceneComponent::SceneComponent(AActor& owner, int update_order)
		:ActorComponent{owner, update_order}
	{
	}

	void SceneComponent::AttachTo(SceneComponent* new_parent)
	{
		if (parent_)
		{
			auto& siblings = parent_->childs_;
			const auto me = std::find_if(siblings.begin(), siblings.end(), 
				[this](const SceneComponent& other) { return &other == this; }
			);
			
			IF_ENSURE_MSG(me != siblings.end(), "This component was not in the parent's child list")
			{
				siblings.erase(me);
			}
		}

		parent_ = new_parent;

		if (new_parent)
		{
			new_parent->childs_.emplace_back(*this);
		}
	}

	void SceneComponent::SetRelTransform(const Transform& new_transform, bool recalc_world_transform) noexcept
	{
		rel_transform_ = new_transform;
		if (recalc_world_transform) RecalcWorldTransform();
	}

	void SceneComponent::SetRelPos(const Vec3& new_pos, bool recalc_world_transform) noexcept
	{
		rel_transform_.pos = new_pos;
		if (recalc_world_transform) RecalcWorldTransform();
	}

	void SceneComponent::SetRelRot(const Quat& new_rot, bool recalc_world_transform) noexcept
	{
		rel_transform_.rot = new_rot;
		if (recalc_world_transform) RecalcWorldTransform();
	}

	void SceneComponent::SetRelScale(const Vec3& scale, bool recalc_world_transform) noexcept
	{
		rel_transform_.scale = scale;
		if (recalc_world_transform) RecalcWorldTransform();
	}

	void SceneComponent::RecalcWorldTransform(bool propagate) noexcept
	{
		if (parent_) world_transform_ = parent_->GetWorldTransform() * rel_transform_.ToMatrix();
		else world_transform_ = rel_transform_.ToMatrix();

		if (!propagate) return;

		for (auto&& c : childs_)
		{
			c.get().RecalcWorldTransform(propagate);
		}
	}
}
