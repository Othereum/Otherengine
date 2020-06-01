#pragma once
#include "ActorComponent.hpp"
#include "Math.hpp"
#include "Templates/DyArr.hpp"

namespace oeng
{
	class SceneComponent : public ActorComponent
	{
	public:
		explicit SceneComponent(AActor& owner, int update_order = 100);

		void AttachTo(const SharedPtr<SceneComponent>& new_parent);

		void SetRelTransform(const Transform& new_transform, bool recalc_world_transform = true) noexcept;
		[[nodiscard]] const Transform& GetRelTransform() const noexcept { return rel_transform_; }

		void SetRelPos(const Vec3& new_pos, bool recalc_world_transform = true) noexcept;
		[[nodiscard]] const Vec3& GetRelPos() const noexcept { return rel_transform_.pos; }

		void SetRelRot(const Quat& new_rot, bool recalc_world_transform = true) noexcept;
		[[nodiscard]] const Quat& GetRelRot() const noexcept { return rel_transform_.rot; }

		void SetRelScale(const Vec3& scale, bool recalc_world_transform = true) noexcept;
		[[nodiscard]] const Vec3& GetRelScale() const noexcept { return rel_transform_.scale; }

		void RecalcWorldTransform(bool propagate = true) noexcept;
		[[nodiscard]] const Mat4& GetWorldTransform() const noexcept { return world_transform_; }

	private:
		WeakPtr<SceneComponent> parent_;
		DyArr<WeakPtr<SceneComponent>> childs_;
		Transform rel_transform_;
		Mat4 world_transform_;
	};
}
 