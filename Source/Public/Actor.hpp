#pragma once
#include <vector>
#include "Templates/SharedPtr.hpp"
#include "TimerManager.hpp"
#include "Math.hpp"

namespace oeng
{
	class Engine;
	class CWorld;
	class ActorComponent;
	class SceneComponent;
	class TimerManager;
	
	class AActor : public EnableSharedFromThis<AActor>
	{
	public:
		explicit AActor(CWorld& world);
		virtual ~AActor();

		void BeginPlay();
		void Update(float delta_seconds);
		void Destroy();

		template <std::derived_from<ActorComponent> T, class... Args>
		WeakPtr<T> AddComponent(Args&&... args)
		{
			auto ptr = MakeShared<T>(*this, std::forward<Args>(args)...);
			WeakPtr weak = ptr;
			RegisterComponent(std::move(ptr));
			return weak;
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

		void SetTransform(const Transform& new_transform, bool recalc_matrix = true) const noexcept;
		[[nodiscard]] const Transform& GetTransform() const noexcept;
		
		void SetPos(const Vec3& new_pos, bool recalc_matrix = true) const noexcept;
		[[nodiscard]] const Vec3& GetPos() const noexcept;

		void SetRot(const Quat& new_rot, bool recalc_matrix = true) const noexcept;
		[[nodiscard]] const Quat& GetRot() const noexcept;

		void SetScale(const Vec3& scale, bool recalc_matrix = true) const noexcept;
		[[nodiscard]] const Vec3& GetScale() const noexcept;

		void RecalcMatrix() const noexcept;
		[[nodiscard]] const Mat4& GetTransformMatrix() const noexcept;

		[[nodiscard]] Vec3 GetForward() const noexcept { return Vec3::Forward().RotatedBy(GetRot()); }

		[[nodiscard]] Engine& GetEngine() const noexcept;
		[[nodiscard]] CWorld& GetWorld() const noexcept { return world_; }
		[[nodiscard]] TimerManager& GetTimerManager() const noexcept;

	private:
		void RegisterComponent(SharedPtr<ActorComponent>&& comp);
		void UpdateComponents(float delta_seconds);
		virtual void UpdateActor(float delta_seconds) {}

		bool pending_kill_ : 1 = false;
		bool begun_play_ : 1 = false;
		
		float init_lifespan_ = 0;
		TimerHandle lifespan_timer_;
		
		CWorld& world_;
		std::vector<SharedPtr<ActorComponent>> comps_;
		SceneComponent* root_;
	};
}
