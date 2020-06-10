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

		
		void SetRelTrsf(const Transform& trsf) noexcept { rel_trsf_ = trsf; RecalcWorldTrsf(); }
		[[nodiscard]] const Transform& GetRelTrsf() const noexcept { return rel_trsf_; }

		void SetRelPos(const Vec3& pos) noexcept { rel_trsf_.pos = pos; RecalcWorldTrsf(); }
		[[nodiscard]] const Vec3& GetRelPos() const noexcept { return rel_trsf_.pos; }

		void SetRelRot(const Quat& rot) noexcept { rel_trsf_.rot = rot; RecalcWorldTrsf(); }
		[[nodiscard]] const Quat& GetRelRot() const noexcept { return rel_trsf_.rot; }

		void SetRelScale(const Vec3& scale) noexcept { rel_trsf_.scale = scale; RecalcWorldTrsf(); }
		[[nodiscard]] const Vec3& GetRelScale() const noexcept { return rel_trsf_.scale; }

		
		void SetWorldTransform(const Transform& trsf) noexcept { world_trsf_ = trsf; RecalcRelTrsf(); }
		[[nodiscard]] const Transform& GetWorldTransform() const noexcept { return world_trsf_; }

		void SetWorldPos(const Vec3& pos) noexcept { world_trsf_.pos = pos; RecalcRelTrsf(); }
		[[nodiscard]] const Vec3& GetWorldPos() const noexcept { return world_trsf_.pos; }

		void SetWorldRot(const Quat& rot) noexcept { world_trsf_.rot = rot; RecalcRelTrsf(); }
		[[nodiscard]] const Quat& GetWorldRot() const noexcept { return world_trsf_.rot; }

		void SetWorldScale(const Vec3& scale) noexcept { world_trsf_.scale = scale; RecalcRelTrsf(); }
		[[nodiscard]] const Vec3& GetWorldScale() const noexcept { return world_trsf_.scale; }

		
		[[nodiscard]] const Mat4& GetWorldMatrix() const noexcept { return world_mat_; }
		
	private:
		// Recalculate world transform with relative transform
		void RecalcWorldTrsf() noexcept;

		// Recalculate relative transform with world transform
		void RecalcRelTrsf() noexcept;
		
		virtual void OnTrsfChanged() {}
		
		SceneComponent* parent_;
		DyArr<std::reference_wrapper<SceneComponent>> childs_;
		Transform rel_trsf_;
		Transform world_trsf_;
		Mat4 world_mat_;
	};
}
 