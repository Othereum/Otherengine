#pragma once
#include "TimerManager.hpp"
#include "Math.hpp"
#include "Templates/DyArr.hpp"
#include "Templates/Memory.hpp"

namespace oeng
{
	class Engine;
	class World;
	class ActorComponent;
	class SceneComponent;
	class TimerManager;
	
	class OEAPI AActor : public EnableSharedFromThis<AActor>
	{
	public:
		explicit AActor(World& world);
		virtual ~AActor();

		void BeginPlay();
		void Update(float delta_seconds);
		void Destroy();

		template <class T, class... Args>
		T& AddComponent(Args&&... args)
		{
			static_assert(std::is_base_of_v<ActorComponent, T>);
			auto ptr = MakeShared<T>(*this, std::forward<Args>(args)...);
			auto& ref = *ptr;
			RegisterComponent(std::move(ptr));
			return ref;
		}

		/**
		 * \brief Set root component of this actor. Root component represents this actor's transform.
		 * \param new_root New root component. It can be nullptr or MUST be owned by this actor. 
		 */
		void SetRootComponent(SceneComponent* new_root) noexcept;
		[[nodiscard]] SceneComponent* GetRootComponent() const noexcept { return root_; }

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

		[[nodiscard]] Vec3 GetForward() const noexcept { return Vec3::forward.RotatedBy(GetRot()); }

		[[nodiscard]] Engine& GetEngine() const noexcept;
		[[nodiscard]] World& GetWorld() const noexcept { return world_; }
		[[nodiscard]] TimerManager& GetTimerManager() const noexcept;

	private:
		void RegisterComponent(SharedPtr<ActorComponent>&& comp);
		void UpdateComponents(float delta_seconds);
		virtual void OnUpdate(float delta_seconds) {}
		virtual void OnBeginPlay() {}

		bool pending_kill_ : 1 = false;
		bool begun_play_ : 1 = false;
		
		float init_lifespan_ = 0;
		TimerHandle lifespan_timer_;
		
		World& world_;
		DyArr<SharedPtr<ActorComponent>> comps_;
		SceneComponent* root_ = nullptr;
	};
}
