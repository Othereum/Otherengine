#include "Components/SceneComponent.hpp"
#include "Assert.hpp"

namespace oeng
{
	SceneComponent::SceneComponent(AActor& owner, int update_order)
		:ActorComponent{owner, update_order}, parent_{}
	{
	}

	void SceneComponent::AttachTo(SceneComponent* new_parent, AttachRule rule)
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

		switch (rule)
		{
		case AttachRule::kKeepRelative: RecalcWorldTrsf(); break;
		case AttachRule::kKeepWorld: RecalcRelTrsf(); break;
		default: throw std::invalid_argument{"Unknown attach rule"};
		}
	}

	void SceneComponent::RecalcWorldTrsf() noexcept
	{
		world_mat_ = parent_
			? parent_->GetWorldMatrix() * rel_trsf_.ToMatrix()
			: rel_trsf_.ToMatrix();

		world_trsf_ = Transform{world_mat_};

		for (auto c : childs_)
		{
			c.get().RecalcWorldTrsf();
		}

		OnTrsfChanged();
	}

	void SceneComponent::RecalcRelTrsf() noexcept
	{
		world_mat_ = world_trsf_.ToMatrix();
		
		if (parent_)
		{
			if (const auto pwi = parent_->GetWorldMatrix().Inv())
			{
				// pw: parent's world transform
				// r: unknown new relative transform to parent
				// w: desired world transform
				// 
				// pw * r = w
				// pw(-1) * pw * r = pw(-1) * w
				// r = pw(-1) * w
				rel_trsf_ = Transform{*pwi * world_mat_};
			}
			else
			{
				rel_trsf_ = world_trsf_;
			}
		}
		else
		{
			rel_trsf_ = world_trsf_;
		}

		for (auto c : childs_)
		{
			c.get().RecalcWorldTrsf();
		}

		OnTrsfChanged();
	}
}
