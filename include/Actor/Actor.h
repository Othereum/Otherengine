#pragma once

#include <memory>
#include <vector>
#include "vector.h"
#include "rotation.h"

namespace Game
{
	class CActorComponent;
	class CEngine;
	
	class AActor
	{
	public:
		enum class EState
		{
			active, paused, dead
		};

		explicit AActor(CEngine& engine);
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

		void SetLifespan(float seconds) { lifespan_ = seconds; }

		void SetPos(TFVector2 new_pos) noexcept { pos_ = new_pos; }
		[[nodiscard]] TFVector2 GetPos() const noexcept { return pos_; }

		void SetRot(TDegrees new_rot) noexcept { rot_ = new_rot; }
		[[nodiscard]] TDegrees GetRot() const noexcept { return rot_; }
		[[nodiscard]] TFVector2 GetForward() const noexcept;

		[[nodiscard]] EState GetState() const noexcept { return state_; }
		
		void SetScale(float scale) noexcept { scale_ = scale; }
		[[nodiscard]] float GetScale() const noexcept { return scale_; }

		[[nodiscard]] CEngine& GetEngine() const noexcept { return engine_; }

	private:
		void RegisterComponent(std::unique_ptr<CActorComponent>&& comp);
		void UpdateComponents(float deltaSeconds);
		void UpdateLifespan(float deltaSeconds);
		virtual void UpdateActor(float deltaSeconds) {}

		EState state_ = EState::active;
		float lifespan_ = 0;
		
		TFVector2 pos_;
		TDegrees rot_;
		float scale_ = 1;
		
		CEngine& engine_;
		std::vector<std::unique_ptr<CActorComponent>> comps_;
	};
}