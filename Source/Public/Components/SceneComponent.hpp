#pragma once
#include "ActorComponent.hpp"
#include <memory>
#include <vector>
#include "Math.hpp"

namespace oeng
{
	class SceneComponent : public ActorComponent
	{
	public:
		explicit SceneComponent(AActor& owner, int update_order = 100);

		void SetRelTransform(const Transform& new_transform, bool recalc_matrix = true) noexcept;
		[[nodiscard]] const Transform& GetRelTransform() const noexcept;

		void SetRelPos(const Vec3& new_pos, bool recalc_matrix = true) noexcept;
		[[nodiscard]] const Vec3& GetRelPos() const noexcept { return rel_transform_.pos; }

		void SetRelRot(const Quat& new_rot, bool recalc_matrix = true) noexcept;
		[[nodiscard]] const Quat& GetRelRot() const noexcept { return rel_transform_.rot; }

		void SetRelScale(const Vec3& scale, bool recalc_matrix = true) noexcept;
		[[nodiscard]] const Vec3& GetRelScale() const noexcept { return rel_transform_.scale; }

		void RecalcMatrix() noexcept;
		[[nodiscard]] const Mat4& GetTransformMatrix() const noexcept { return transform_matrix_; }

	private:
		SceneComponent* parent_;
		std::vector<SceneComponent*> childs_;
		Transform rel_transform_;
		Mat4 transform_matrix_;
	};
}
