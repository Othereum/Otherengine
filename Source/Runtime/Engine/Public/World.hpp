#pragma once
#include "EngineFwd.hpp"
#include "TimerManager.hpp"
#include "Templates/DyArr.hpp"
#include "Templates/Pointer.hpp"
#include "Templates/Time.hpp"

namespace oeng::engine
{
	class ENGINE_API World
	{
	public:
		DELETE_CPMV(World);
		
		World();
		~World();

		void BeginTick();
		void Tick();
		
		template <std::derived_from<AActor> T, class... Args>
		SharedRef<T> SpawnActor(Args&&... args)
		{
			auto ptr = MakeShared<T>(*this, std::forward<Args>(args)...);
			pending_actors_.push_back(ptr);
			return ptr;
		}

		template <std::invocable<AActor&> Fn>
		void ForEachActor(Fn&& fn) const
		{
			for (auto& actor : actors_)
			{
				fn(*actor);
			}
		}

		void RegisterCollision(SphereComponent& comp);
		void UnregisterCollision(SphereComponent& comp);
		
		[[nodiscard]] TimerManager& GetTimerManager() noexcept { return timer_; }
		[[nodiscard]] TimePoint GetTime() const noexcept { return time_; }
		[[nodiscard]] Float GetDeltaSeconds() const noexcept { return delta_seconds_; }

	private:
		void UpdateGame();
		void UpdateTime();

		TimerManager timer_;

		DyArr<std::reference_wrapper<SphereComponent>> collisions_;
		
		DyArr<SharedRef<AActor>> actors_;
		DyArr<SharedRef<AActor>> pending_actors_;
		
		TimePoint time_;
		Float delta_seconds_;
	};
}
