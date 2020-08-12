#pragma once

namespace oeng
{
	namespace engine
	{
		struct TimerHandle;
	}
	using namespace engine;
}

#ifndef DOXYGEN_SHOULD_SKIP_THIS
template <>
struct ENGINE_API std::hash<oeng::TimerHandle>
{
	size_t operator()(oeng::TimerHandle key) const noexcept;
};
#endif

namespace oeng::engine
{
	enum class Loop
	{
		kStop, kContinue
	};
	
	struct ENGINE_API TimerHandle
	{
		constexpr bool operator==(const TimerHandle&) const noexcept = default;
		
	private:
		friend class TimerManager;
		friend std::hash<TimerHandle>;
		
		static TimerHandle Create() noexcept;
		
		size_t key = 0;
	};

	class ENGINE_API TimerManager
	{
	public:
		DELETE_CPMV(TimerManager);
		
		explicit TimerManager(class World& world) noexcept;
		~TimerManager();
		
		void Update();

		TimerHandle SetLoopTimer(Float delay_in_seconds, std::function<Loop()>&& fn = []{return Loop::kContinue;});
		TimerHandle SetTimer(Float delay_in_seconds, std::function<void()>&& fn = []{});
		
		/**
		 * Set timer for next tick. Technically, fn is called at the end of the tick.
		 * @param fn Callback function
		 */
		void SetTimerForNextTick(std::function<void()>&& fn);
		
		/**
		 * Update timer
		 * @param handle Handle of timer to be updated
		 * @param new_delay New delay in seconds
		 * @param restart Should the timer be restarted based on the current time?
		 * @return If the timer exists and updated successfully
		 */
		bool UpdateTimer(TimerHandle handle, Float new_delay, bool restart = true) noexcept;
		
		/**
		 * Remove timer
		 * @param handle Handle of timer
		 * @return If the timer existed and removed successfully
		 */
		bool RemoveTimer(TimerHandle handle) noexcept;
		
		/**
		 * Get remaining time of timer
		 * @param handle Handle of timer
		 * @return Remaining time. If timer not exists, returns 0
		 */
		[[nodiscard]] Float TimeLeft(TimerHandle handle) const noexcept;
		[[nodiscard]] bool IsTimerExists(TimerHandle handle) const noexcept;

	private:
		struct FTimer
		{
			std::function<Loop()> fn;
			Clock::time_point end;
			Clock::duration delay;
		};
		
		World& world_;
		std::unordered_map<TimerHandle, FTimer> timers_;
		std::unordered_map<TimerHandle, FTimer> pending_timers_;
	};
}
