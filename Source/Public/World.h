#pragma once
#include <chrono>
#include <memory>
#include <vector>

namespace oeng
{
	using Clock = std::chrono::steady_clock;
	
	class CEngine;
	class TimerManager;
	class AActor;
	class CInputComponent;
	class CCircleComponent;

	class CWorld
	{
	public:
		explicit CWorld(CEngine& engine);
		~CWorld();
		
		template <class T>
		std::shared_ptr<T> SpawnActor()
		{
			static_assert(std::is_base_of_v<AActor, T>);
			auto ptr = std::make_shared<T>(*this);
			RegisterActor(ptr);
			return ptr;
		}

		void Tick();

		void RegisterInputComponent(const CInputComponent& comp);
		void UnregisterInputComponent(const CInputComponent& comp);

		void RegisterCollision(CCircleComponent& comp);
		void UnregisterCollision(CCircleComponent& comp);
		
		[[nodiscard]] CEngine& GetEngine() const noexcept { return engine_; }
		[[nodiscard]] TimerManager& GetTimerManager() const noexcept { return *timer_; }
		[[nodiscard]] Clock::time_point GetTime() const noexcept { return time_; }
		[[nodiscard]] float GetDeltaSeconds() const noexcept { return delta_seconds_; }

		CWorld(const CWorld&) = delete;
		CWorld(CWorld&&) = delete;
		CWorld& operator=(const CWorld&) = delete;
		CWorld& operator=(CWorld&&) = delete;

	private:
		void UpdateGame();
		void UpdateTime();
		void RegisterActor(std::shared_ptr<AActor> actor);

		CEngine& engine_;
		std::unique_ptr<TimerManager> timer_;

		std::vector<std::reference_wrapper<CCircleComponent>> collisions_;
		
		std::vector<std::shared_ptr<AActor>> actors_;
		std::vector<std::shared_ptr<AActor>> pending_actors_;
		
		Clock::time_point time_;
		float delta_seconds_;
	};
}
