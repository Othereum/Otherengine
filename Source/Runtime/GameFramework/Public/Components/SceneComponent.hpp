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

		void SetRelTransform(const Transform& transform) noexcept { rel_transform_ = transform; RecalcWorldTransform(); }
		[[nodiscard]] const Transform& GetRelTransform() const noexcept { return rel_transform_; }

		void SetRelPos(const Vec3& new_pos) noexcept { rel_transform_.pos = new_pos; RecalcWorldTransform(); }
		[[nodiscard]] const Vec3& GetRelPos() const noexcept { return rel_transform_.pos; }

		void SetRelRot(const Quat& new_rot) noexcept { rel_transform_.rot = new_rot; RecalcWorldTransform(); }
		[[nodiscard]] const Quat& GetRelRot() const noexcept { return rel_transform_.rot; }

		void SetRelScale(const Vec3& scale) noexcept { rel_transform_.scale = scale; RecalcWorldTransform(); }
		[[nodiscard]] const Vec3& GetRelScale() const noexcept { return rel_transform_.scale; }

		void RecalcWorldTransform(bool propagate = true) noexcept;
		[[nodiscard]] const Mat4& GetWorldTransform() const noexcept { return world_transform_; }

	private:
		SceneComponent* parent_;
		DyArr<std::reference_wrapper<SceneComponent>> childs_;
		Transform rel_transform_;
		Mat4 world_transform_;
	};
}
 