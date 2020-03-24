#pragma once

#include <memory>
#include <vector>
#include "Vector.h"
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

		void SetPos(FVector2 new_pos) noexcept { pos_ = new_pos; }
		[[nodiscard]] FVector2 GetPos() const noexcept { return pos_; }

		void SetRot(Degrees new_rot) noexcept { rot_ = new_rot; }
		[[nodiscard]] Degrees GetRot() const noexcept { return rot_; }
		[[nodiscard]] FVector2 GetForward() const noexcept;

		[[nodiscard]] EState GetState() const noexcept { return state_; }
		
		void SetScale(float scale) noexcept { scale_ = scale; }
		[[nodiscard]] float GetScale() const noexcept { return scale_; }

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
		
		FVector2 pos_;
		Degrees rot_;
		float scale_ = 1;
		
		CWorld& world_;
		std::vector<std::unique_ptr<CActorComponent>> comps_;
	};
}