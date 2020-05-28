#pragma once
#include <chrono>
#include <memory>
#include <vector>

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
		std::weak_ptr<T> SpawnActor()
		{
			static_assert(std::is_base_of_v<AActor, T>);
			auto actor = std::make_shared<T>(*this);
			std::weak_ptr weak = actor;
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
		void RegisterActor(std::shared_ptr<AActor>&& actor);

		Engine& engine_;
		std::unique_ptr<TimerManager> timer_;

		std::vector<std::reference_wrapper<CircleComponent>> collisions_;
		
		std::vector<std::shared_ptr<AActor>> actors_;
		std::vector<std::shared_ptr<AActor>> pending_actors_;
		
		Clock::time_point time_;
		float delta_seconds_;
	};
}
