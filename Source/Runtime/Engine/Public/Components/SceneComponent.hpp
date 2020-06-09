#pragma once
#include "ActorComponent.hpp"
#include "Math.hpp"
#include "Templates/DyArr.hpp"

namespace oeng
{
	class OEAPI SceneComponent : public ActorComponent
	{
	public:
		explicit SceneComponent(AActor& owner, int update_order = 100);

		void AttachTo(SceneComponent* new_parent);

		void SetRelTransform(const Transform& transform) noexcept { rel_trsf_ = transform; TransformChanged(); }
		[[nodiscard]] const Transform& GetRelTransform() const noexcept { return rel_trsf_; }

		void SetRelPos(const Vec3& new_pos) noexcept { rel_trsf_.pos = new_pos; TransformChanged(); }
		[[nodiscard]] const Vec3& GetRelPos() const noexcept { return rel_trsf_.pos; }

		void SetRelRot(const Quat& new_rot) noexcept { rel_trsf_.rot = new_rot; TransformChanged(); }
		[[nodiscard]] const Quat& GetRelRot() const noexcept { return rel_trsf_.rot; }

		void SetRelScale(const Vec3& scale) noexcept { rel_trsf_.scale = scale; TransformChanged(); }
		[[nodiscard]] const Vec3& GetRelScale() const noexcept { return rel_trsf_.scale; }

		void RecalcWorldTransform(bool propagate = true) noexcept;
		[[nodiscard]] const Transform& GetWorldTransform() const noexcept { return world_trsf_; }
		[[nodiscard]] const Mat4& GetWorldMatrix() const noexcept { return world_mat_; }

	private:
		void TransformChanged() { RecalcWorldTransform(); OnTransformChanged(); }
		virtual void OnTransformChanged() {}
		
		SceneComponent* parent_;
		DyArr<std::reference_wrapper<SceneComponent>> childs_;
		Transform rel_trsf_;
		Transform world_trsf_;
		Mat4 world_mat_;
	};
}
 