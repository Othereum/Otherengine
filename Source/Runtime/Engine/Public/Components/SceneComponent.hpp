#pragma once
#include "ActorComponent.hpp"
#include "Math.hpp"
#include "Templates/DyArr.hpp"

namespace oeng
{
	enum class AttachRule
	{
		kKeepRelative, kKeepWorld
	};
	
	class OEAPI SceneComponent : public ActorComponent
	{
	public:
		explicit SceneComponent(AActor& owner, int update_order = 100);

		void AttachTo(SceneComponent* new_parent, AttachRule rule);

		void SetRelTrsf(const Transform& trsf) noexcept { rel_trsf_ = trsf; RecalcWorldTrsf(); }
		void SetRelPos(const Vec3& pos) noexcept { rel_trsf_.pos = pos; RecalcWorldTrsf(); }
		void SetRelRot(const Quat& rot) noexcept { rel_trsf_.rot = rot; RecalcWorldTrsf(); }
		void SetRelScale(const Vec3& scale) noexcept { rel_trsf_.scale = scale; RecalcWorldTrsf(); }
		void SetWorldTransform(const Transform& trsf) noexcept { world_trsf_ = trsf; RecalcRelTrsf(); }
		void SetWorldPos(const Vec3& pos) noexcept { world_trsf_.pos = pos; RecalcRelTrsf(); }
		void SetWorldRot(const Quat& rot) noexcept { world_trsf_.rot = rot; RecalcRelTrsf(); }
		void SetWorldScale(const Vec3& scale) noexcept { world_trsf_.scale = scale; RecalcRelTrsf(); }
				
		[[nodiscard]] const Transform& GetRelTrsf() const noexcept { return rel_trsf_; }
		[[nodiscard]] const Vec3& GetRelPos() const noexcept { return rel_trsf_.pos; }
		[[nodiscard]] const Quat& GetRelRot() const noexcept { return rel_trsf_.rot; }
		[[nodiscard]] const Vec3& GetRelScale() const noexcept { return rel_trsf_.scale; }
		[[nodiscard]] const Transform& GetWorldTransform() const noexcept { return world_trsf_; }
		[[nodiscard]] const Vec3& GetWorldPos() const noexcept { return world_trsf_.pos; }
		[[nodiscard]] const Quat& GetWorldRot() const noexcept { return world_trsf_.rot; }
		[[nodiscard]] const Vec3& GetWorldScale() const noexcept { return world_trsf_.scale; }

		[[nodiscard]] Vec3 GetForward() const noexcept { return Vec3::forward.RotatedBy(world_trsf_.rot); }
		[[nodiscard]] Vec3 GetBackward() const noexcept { return Vec3::backward.RotatedBy(world_trsf_.rot); }
		[[nodiscard]] Vec3 GetRight() const noexcept { return Vec3::right.RotatedBy(world_trsf_.rot); }
		[[nodiscard]] Vec3 GetLeft() const noexcept { return Vec3::left.RotatedBy(world_trsf_.rot); }
		[[nodiscard]] Vec3 GetUp() const noexcept { return Vec3::up.RotatedBy(world_trsf_.rot); }
		[[nodiscard]] Vec3 GetDown() const noexcept { return Vec3::down.RotatedBy(world_trsf_.rot); }

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
		Mat4 world_mat_ = Mat4::identity;
	};
}
 