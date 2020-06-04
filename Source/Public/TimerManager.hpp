#pragma once
#include "Templates/Function.hpp"
#include "Templates/HashMap.hpp"

namespace oeng
{
	struct TimerHandle;
}

template <>
struct OEAPI std::hash<oeng::TimerHandle>
{
	size_t operator()(oeng::TimerHandle key) const noexcept;
};

namespace oeng
{
	enum class Loop
	{
		kStop, kContinue
	};
	
	struct OEAPI TimerHandle
	{
		constexpr bool operator==(const TimerHandle&) const noexcept = default;
		
	private:
		friend class TimerManager;
		friend std::hash<TimerHandle>;
		
		static TimerHandle Create() noexcept;
		
		size_t key = 0;
	};

	class OEAPI TimerManager
	{
	public:
		explicit TimerManager(class World& world) noexcept;
		~TimerManager();
		
		void Update();

		TimerHandle SetLoopTimer(float delay_in_seconds, Function<Loop()>&& fn = []{return Loop::kContinue;});
		TimerHandle SetTimer(float delay_in_seconds, Function<void()>&& fn = []{});
		
		/**
		 * \brief Set timer for next tick. Technically, fn is called at the end of the tick.
		 * \param fn Callback function
		 */
		void SetTimerForNextTick(Function<void()>&& fn);
		
		/**
		 * \brief Update timer
		 * \param handle Handle of timer to be updated
		 * \param new_delay New delay in seconds
		 * \param restart Should the timer be restarted based on the current time?
		 * \return If the timer exists and updated successfully
		 */
		bool UpdateTimer(TimerHandle handle, float new_delay, bool restart = true) noexcept;
		
		/**
		 * \brief Remove timer
		 * \param handle Handle of timer
		 * \return If the timer existed and removed successfully
		 */
		bool RemoveTimer(TimerHandle handle) noexcept;
		
		/**
		 * \brief Get remaining time of timer
		 * \param handle Handle of timer
		 * \return Remaining time. If timer not exists, returns 0
		 */
		[[nodiscard]] float TimeLeft(TimerHandle handle) const noexcept;
		[[nodiscard]] bool IsTimerExists(TimerHandle handle) const noexcept;

		TimerManager(const TimerManager&) = delete;
		TimerManager(TimerManager&&) = delete;
		TimerManager& operator=(const TimerManager&) = delete;
		TimerManager& operator=(TimerManager&&) = delete;

	private:
		struct FTimer;
		
		World& world_;
		HashMap<TimerHandle, FTimer> timers_;
		HashMap<TimerHandle, FTimer> pending_timers_;
	};
}
