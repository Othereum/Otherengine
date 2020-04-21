#pragma once

#include <memory>
#include <vector>
#include "Rotation.h"
#include "Transform.hpp"

namespace oeng
{
	class CEngine;
	class CWorld;
	class CActorComponent;
	class CTimerManager;
	
	class AActor
	{
	public:
		explicit AActor(CWorld& world);
		virtual ~AActor();

		void BeginPlay() const;
		void Update(float delta_seconds);
		void Destroy();

		template <class T, class... Args>
		T& AddComponent(Args&&... args)
		{
			static_assert(std::is_base_of_v<CActorComponent, T>);
			auto ptr = std::make_unique<T>(*this, std::forward<Args>(args)...);
			auto& ref = *ptr;
			RegisterComponent(std::move(ptr));
			return ref;
		}

		void SetEnabled(bool enable);

		void SetLifespan(float seconds) { lifespan_ = seconds; }
		[[nodiscard]] bool IsPendingKill() const noexcept { return pending_kill_; }

		void SetTransform(const Transform& new_transform, bool recompute_matrix = true) noexcept;
		[[nodiscard]] const Transform& GetTransform() const noexcept { return world_transform_; }
		
		void SetPos(const Vec2& new_pos, bool recompute_matrix = true) noexcept;
		[[nodiscard]] const Vec2& GetPos() const noexcept { return world_transform_.pos; }

		void SetRot(Degrees new_rot, bool recompute_matrix = true) noexcept;
		[[nodiscard]] Degrees GetRot() const noexcept { return world_transform_.rot; }
		[[nodiscard]] Vec2 GetForward() const noexcept;

		void SetScale(const Vec2& scale, bool recompute_matrix = true) noexcept;
		[[nodiscard]] const Vec2& GetScale() const noexcept { return world_transform_.scale; }

		void RecomputeMatrix() noexcept;
		[[nodiscard]] const Mat4& GetTransformMatrix() const noexcept { return transform_matrix_; }

		[[nodiscard]] CEngine& GetEngine() const noexcept;
		[[nodiscard]] CWorld& GetWorld() const noexcept { return world_; }
		[[nodiscard]] CTimerManager& GetTimerManager() const noexcept;

	private:
		void RegisterComponent(std::unique_ptr<CActorComponent>&& comp);
		void UpdateComponents(float delta_seconds);
		void UpdateLifespan(float delta_seconds);
		virtual void UpdateActor(float delta_seconds) {}

		bool pending_kill_ = false;
		float lifespan_ = 0;

		Transform world_transform_;
		Mat4 transform_matrix_;
		
		CWorld& world_;
		std::vector<std::unique_ptr<CActorComponent>> comps_;
	};
}
