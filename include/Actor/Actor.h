#pragma once

#include <memory>
#include <vector>
#include "Algebra.h"
#include "Rotation.h"

namespace game
{
	class CEngine;
	class CWorld;
	class CActorComponent;
	class CTimerManager;
	
	class AActor
	{
	public:
		enum class EState
		{
			active, paused, dead
		};

		explicit AActor(CWorld& world);
		virtual ~AActor();

		void BeginPlay() const;
		void Update(float deltaSeconds);
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
		[[nodiscard]] EState GetState() const noexcept { return state_; }
		
		void SetPos(const Vector3f& new_pos, bool recompute_world_transform = true) noexcept;
		[[nodiscard]] const Vector3f& GetPos() const noexcept { return pos_; }

		void SetRot(Degrees new_rot, bool recompute_world_transform = true) noexcept;
		[[nodiscard]] Degrees GetRot() const noexcept { return rot_; }
		[[nodiscard]] Vector3f GetForward() const noexcept;

		void SetScale(const Vector3f& scale, bool recompute_world_transform = true) noexcept;
		[[nodiscard]] const Vector3f& GetScale() const noexcept { return scale_; }

		void RecomputeWorldTransform() noexcept;

		[[nodiscard]] CEngine& GetEngine() const noexcept;
		[[nodiscard]] CWorld& GetWorld() const noexcept { return world_; }
		[[nodiscard]] CTimerManager& GetTimerManager() const noexcept;

	private:
		void RegisterComponent(std::unique_ptr<CActorComponent>&& comp);
		void UpdateComponents(float deltaSeconds);
		void UpdateLifespan(float deltaSeconds);
		virtual void UpdateActor(float deltaSeconds) {}

		EState state_ = EState::active;
		float lifespan_ = 0;
		
		Vector3f pos_;
		Degrees rot_;
		Vector3f scale_ = Vector3f::Ones();

		Matrix4f world_transform_;
		
		CWorld& world_;
		std::vector<std::unique_ptr<CActorComponent>> comps_;
	};
}