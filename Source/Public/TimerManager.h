#pragma once
#include <functional>
#include <unordered_map>

namespace oeng
{
	struct TimerHandle;
}

template <>
struct std::hash<oeng::TimerHandle>
{
	size_t operator()(oeng::TimerHandle key) const noexcept;
};

namespace oeng
{
	enum class Loop
	{
		kStop, kContinue
	};
	
	struct TimerHandle
	{
		constexpr bool operator==(const TimerHandle&) const noexcept = default;
		
	private:
		friend class TimerManager;
		friend std::hash<TimerHandle>;
		
		static TimerHandle Create() noexcept;
		
		size_t key = 0;
	};

	class TimerManager
	{
	public:
		explicit TimerManager(class CWorld& world);
		~TimerManager();
		
		void Update();

		TimerHandle SetLoopTimer(float delay_in_seconds, std::function<Loop()>&& fn = []{return Loop::kContinue;});
		TimerHandle SetTimer(float delay_in_seconds, std::function<void()>&& fn = []{});
		void SetTimerForNextTick(std::function<void()>&& fn);
		void UpdateTimer(TimerHandle handle, float new_delay, bool restart = true);
		void RemoveTimer(TimerHandle handle);
		[[nodiscard]] bool IsTimerExists(TimerHandle handle) const noexcept;
		[[nodiscard]] float TimeLeft(TimerHandle handle) const;

		TimerManager(const TimerManager&) = delete;
		TimerManager(TimerManager&&) = delete;
		TimerManager& operator=(const TimerManager&) = delete;
		TimerManager& operator=(TimerManager&&) = delete;

	private:
		struct FTimer;
		
		CWorld& world_;
		std::unordered_map<TimerHandle, FTimer> timers_;
		std::unordered_map<TimerHandle, FTimer> pending_timers_;
	};
}
