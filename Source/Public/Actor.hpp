#pragma once

#include <memory>
#include <vector>
#include "TimerManager.hpp"
#include "Math.hpp"

namespace oeng
{
	class Engine;
	class CWorld;
	class CActorComponent;
	class TimerManager;
	
	class AActor : public std::enable_shared_from_this<AActor>
	{
	public:
		explicit AActor(CWorld& world);
		virtual ~AActor();

		void BeginPlay();
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
		[[nodiscard]] bool IsPendingKill() const noexcept { return pending_kill_; }

		/**
		 * \brief Set actor's lifespan. Default is 0 (infinite). Timer is updated when called.
		 * \param in_seconds New lifespan in seconds. <=0 means infinite.
		 */
		void SetLifespan(float in_seconds);
		[[nodiscard]] float GetLifespan() const noexcept;
		[[nodiscard]] float GetInitialLifespan() const noexcept { return init_lifespan_; }

		void SetTransform(const Transform& new_transform, bool recompute_matrix = true) noexcept;
		[[nodiscard]] const Transform& GetTransform() const noexcept { return world_transform_; }
		
		void SetPos(const Vec3& new_pos, bool recompute_matrix = true) noexcept;
		[[nodiscard]] const Vec3& GetPos() const noexcept { return world_transform_.pos; }

		void SetRot(const Quat& new_rot, bool recompute_matrix = true) noexcept;
		[[nodiscard]] const Quat& GetRot() const noexcept { return world_transform_.rot; }

		void SetScale(const Vec3& scale, bool recompute_matrix = true) noexcept;
		[[nodiscard]] const Vec3& GetScale() const noexcept { return world_transform_.scale; }

		void RecomputeMatrix() noexcept;
		[[nodiscard]] const Mat4& GetTransformMatrix() const noexcept { return transform_matrix_; }

		[[nodiscard]] Vec3 GetForward() const noexcept { return Vec3{1, 0, 0}.Rotated(GetRot()); }

		[[nodiscard]] Engine& GetEngine() const noexcept;
		[[nodiscard]] CWorld& GetWorld() const noexcept { return world_; }
		[[nodiscard]] TimerManager& GetTimerManager() const noexcept;

	private:
		void RegisterComponent(std::unique_ptr<CActorComponent>&& comp);
		void UpdateComponents(float delta_seconds);
		virtual void UpdateActor(float delta_seconds) {}

		bool pending_kill_ : 1 = false;
		bool begun_play_ : 1 = false;
		
		float init_lifespan_ = 0;
		TimerHandle lifespan_timer_;

		Transform world_transform_;
		Mat4 transform_matrix_ = Mat4::Identity();
		
		CWorld& world_;
		std::vector<std::unique_ptr<CActorComponent>> comps_;
	};
}
