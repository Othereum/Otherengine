#include "Components/SceneComponent.hpp"
#include "Assert.hpp"

namespace oeng
{
	SceneComponent::SceneComponent(AActor& owner, int update_order)
		:ActorComponent{owner, update_order}, parent_{}
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

	void SceneComponent::RecalcWorldTransform() noexcept
	{
		world_mat_ = parent_
			? parent_->GetWorldMatrix() * rel_trsf_.ToMatrix()
			: rel_trsf_.ToMatrix();

		world_trsf_ = Transform{world_mat_};

		for (auto c : childs_)
		{
			c.get().RecalcWorldTransform();
		}
	}

	void SceneComponent::SetWorldTransform(const Transform& trsf) noexcept
	{
		// pw: parent's world transform
		// r: unknown new relative transform to parent
		// w: desired world transform
		// 
		// pw * r = w
		// pw(-1) * pw * r = pw(-1) * w
		// r = pw(-1) * w

		world_trsf_ = trsf;
		world_mat_ = trsf.ToMatrix();
		
		if (parent_)
		{
			if (const auto pwi = parent_->GetWorldMatrix().Inv())
			{
				rel_trsf_ = Transform{*pwi * world_mat_};
			}
			else
			{
				rel_trsf_ = trsf;
			}
		}
		else
		{
			rel_trsf_ = trsf;
		}

		for (auto c : childs_)
		{
			c.get().RecalcWorldTransform();
		}

		OnTransformChanged();
	}
}
