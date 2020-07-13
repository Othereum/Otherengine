#pragma once
#include "TimerManager.hpp"
#include "Templates/Time.hpp"
#include "Templates/DyArr.hpp"

namespace oeng
{
	class Engine;
	class AActor;
	class InputComponent;
	class CircleComponent;

	class OEAPI World
	{
	public:
		OE_DELETE_CPMV(World);
		
		explicit World(Engine& engine);
		~World();

		void BeginTick();
		void Tick();
		
		template <class T>
		T& SpawnActor()
		{
			static_assert(std::is_base_of_v<AActor, T>);
			auto ptr = MakeShared<T>(*this);
			auto& ref = *ptr;
			pending_actors_.emplace_back(std::move(ptr));
			return ref;
		}

		void RegisterCollision(CircleComponent& comp);
		void UnregisterCollision(CircleComponent& comp);
		
		[[nodiscard]] Engine& GetEngine() const noexcept { return engine_; }
		[[nodiscard]] TimerManager& GetTimerManager() noexcept { return timer_; }
		[[nodiscard]] TimePoint GetTime() const noexcept { return time_; }
		[[nodiscard]] Float GetDeltaSeconds() const noexcept { return delta_seconds_; }

	private:
		void UpdateGame();
		void UpdateTime();

		Engine& engine_;
		TimerManager timer_;

		DyArr<std::reference_wrapper<CircleComponent>> collisions_;
		
		DyArr<SharedRef<AActor>> actors_;
		DyArr<SharedRef<AActor>> pending_actors_;
		
		TimePoint time_;
		Float delta_seconds_;
	};
}
