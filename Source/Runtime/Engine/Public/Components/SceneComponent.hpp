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

		[[nodiscard]] UVec3 GetForward() const noexcept { return UVec3::forward.RotatedBy(world_trsf_.rot); }
		[[nodiscard]] UVec3 GetBackward() const noexcept { return UVec3::backward.RotatedBy(world_trsf_.rot); }
		[[nodiscard]] UVec3 GetRight() const noexcept { return UVec3::right.RotatedBy(world_trsf_.rot); }
		[[nodiscard]] UVec3 GetLeft() const noexcept { return UVec3::left.RotatedBy(world_trsf_.rot); }
		[[nodiscard]] UVec3 GetUp() const noexcept { return UVec3::up.RotatedBy(world_trsf_.rot); }
		[[nodiscard]] UVec3 GetDown() const noexcept { return UVec3::down.RotatedBy(world_trsf_.rot); }

		[[nodiscard]] const Mat4& GetWorldMatrix() const noexcept { return world_mat_; }
		
	private:
		// Recalculate world transform with relative transform
		void RecalcWorldTrsf() noexcept;

		// Recalculate relative transform with world transform
		void RecalcRelTrsf() noexcept;
		
		virtual void OnTrsfChanged() {}

		/*
		 * SceneComponent이 정적 개체에 사용되는 경우, 매 프레임마다 transform을 재계산할 필요가 없습니다. transform이 변경되는 즉시 재계산을 해주면 됩니다.
		 * 그러나 동적 개체에서는 transform 변경이 빈번히 일어납니다. 그때마다 재계산을 하면 한 프레임에 불필요한 재계산이 여러번 일어날 수 있습니다.
		 * 이를 방지하기 위해 MovementComponent에서 직접 재계산 시점을 결정하도록 합니다.
		 * 
		 * If SceneComponent is used for static objects, it is not necessary to recalculate transforms for each frame. It can just recalculate as soon as transform changes.
		 * However, transform changes occur frequently on dynamic objects. Recalculating each time can cause multiple unnecessary recalculations in one frame.
		 * To prevent this, make MovementComponent to decide itself when to recalculate.
		 */
		friend class MovementComponent;
		
		SceneComponent* parent_;
		DyArr<std::reference_wrapper<SceneComponent>> childs_;
		Transform rel_trsf_;
		Transform world_trsf_;
		Mat4 world_mat_ = Mat4::identity;
	};
}
 