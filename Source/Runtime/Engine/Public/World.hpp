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
		explicit World(Engine& engine);
		~World();
		
		template <class T>
		T& SpawnActor()
		{
			static_assert(std::is_base_of_v<AActor, T>);
			auto actor = MakeShared<T>(*this);
			auto& ref = *actor;
			RegisterActor(std::move(actor));
			return ref;
		}

		void Tick();

		void RegisterInputComponent(const InputComponent& comp);
		void UnregisterInputComponent(const InputComponent& comp);

		void RegisterCollision(CircleComponent& comp);
		void UnregisterCollision(CircleComponent& comp);
		
		[[nodiscard]] Engine& GetEngine() const noexcept { return engine_; }
		[[nodiscard]] TimerManager& GetTimerManager() noexcept { return timer_; }
		[[nodiscard]] Clock::time_point GetTime() const noexcept { return time_; }
		[[nodiscard]] Float GetDeltaSeconds() const noexcept { return delta_seconds_; }

		World(const World&) = delete;
		World(World&&) = delete;
		World& operator=(const World&) = delete;
		World& operator=(World&&) = delete;

	private:
		void UpdateGame();
		void UpdateTime();
		void RegisterActor(SharedPtr<AActor>&& actor);

		Engine& engine_;
		TimerManager timer_;

		DyArr<std::reference_wrapper<CircleComponent>> collisions_;
		
		DyArr<SharedPtr<AActor>> actors_;
		DyArr<SharedPtr<AActor>> pending_actors_;
		
		Clock::time_point time_;
		Float delta_seconds_;
	};
}
