#include "Components/SceneComponent.hpp"

namespace oeng
{
	SceneComponent::SceneComponent(AActor& owner, int update_order)
		:ActorComponent{owner, update_order}
	{
	}

	void SceneComponent::SetRelTransform(const Transform& new_transform, bool recalc_matrix) noexcept
	{
		rel_transform_ = new_transform;
		if (recalc_matrix) RecalcMatrix();
	}

	void SceneComponent::SetRelPos(const Vec3& new_pos, bool recalc_matrix) noexcept
	{
		rel_transform_.pos = new_pos;
		if (recalc_matrix) RecalcMatrix();
	}

	void SceneComponent::SetRelRot(const Quat& new_rot, bool recalc_matrix) noexcept
	{
		rel_transform_.rot = new_rot;
		if (recalc_matrix) RecalcMatrix();
	}

	void SceneComponent::SetRelScale(const Vec3& scale, bool recalc_matrix) noexcept
	{
		rel_transform_.scale = scale;
		if (recalc_matrix) RecalcMatrix();
	}

	void SceneComponent::RecalcMatrix() noexcept
	{
		if (parent_) transform_matrix_ = parent_->GetTransformMatrix() * rel_transform_.ToMatrix();
		else transform_matrix_ = rel_transform_.ToMatrix();

		for (auto c : childs_) c->RecalcMatrix();
	}
}
