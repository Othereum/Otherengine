#pragma once
#include <chrono>

#include "Templates/DyArr.hpp"
#include "Templates/Memory.hpp"

namespace oeng
{
	using Clock = std::chrono::steady_clock;
	
	class Engine;
	class TimerManager;
	class AActor;
	class InputComponent;
	class CircleComponent;

	class World
	{
	public:
		explicit World(Engine& engine);
		~World();
		
		template <class T>
		WeakPtr<T> SpawnActor()
		{
			static_assert(std::is_base_of_v<AActor, T>);
			auto actor = MakeShared<T>(*this);
			WeakPtr weak = actor;
			RegisterActor(std::move(actor));
			return weak;
		}

		void Tick();

		void RegisterInputComponent(const InputComponent& comp);
		void UnregisterInputComponent(const InputComponent& comp);

		void RegisterCollision(CircleComponent& comp);
		void UnregisterCollision(CircleComponent& comp);
		
		[[nodiscard]] Engine& GetEngine() const noexcept { return engine_; }
		[[nodiscard]] TimerManager& GetTimerManager() const noexcept { return *timer_; }
		[[nodiscard]] Clock::time_point GetTime() const noexcept { return time_; }
		[[nodiscard]] float GetDeltaSeconds() const noexcept { return delta_seconds_; }

		World(const World&) = delete;
		World(World&&) = delete;
		World& operator=(const World&) = delete;
		World& operator=(World&&) = delete;

	private:
		void UpdateGame();
		void UpdateTime();
		void RegisterActor(SharedPtr<AActor>&& actor);

		Engine& engine_;
		UniquePtr<TimerManager> timer_;

		DyArr<std::reference_wrapper<CircleComponent>> collisions_;
		
		DyArr<SharedPtr<AActor>> actors_;
		DyArr<SharedPtr<AActor>> pending_actors_;
		
		Clock::time_point time_;
		float delta_seconds_;
	};
}
