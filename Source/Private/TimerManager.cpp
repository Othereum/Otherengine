#include "TimerManager.hpp"
#include <chrono>
#include "World.hpp"

namespace oeng
{
	using namespace std::chrono;

	struct TimerManager::FTimer
	{
		std::function<Loop()> fn;
		Clock::time_point end;
		Clock::duration delay;
	};

	constexpr Clock::duration ToDuration(float sec) noexcept
	{
		return duration_cast<Clock::duration>(duration<float>{sec});
	}

	constexpr float ToFloat(Clock::duration dur) noexcept
	{
		return duration_cast<duration<float>>(dur).count();
	}

	TimerHandle TimerManager::SetLoopTimer(float delay_in_seconds, std::function<Loop()>&& fn)
	{
		const auto handle = TimerHandle::Create();
		const auto delay = ToDuration(delay_in_seconds);
		pending_timers_.insert({handle, {std::move(fn), world_.GetTime() + delay, delay}});
		return handle;
	}

	TimerHandle TimerManager::SetTimer(float delay_in_seconds, std::function<void()>&& fn)
	{
		return SetLoopTimer(delay_in_seconds, [fn = std::move(fn)] { fn(); return Loop::kStop; });
	}

	void TimerManager::SetTimerForNextTick(std::function<void()>&& fn)
	{
		SetTimer(0, std::move(fn));
	}

	bool TimerManager::UpdateTimer(TimerHandle handle, float new_delay, bool restart) noexcept
	{
		const auto it = timers_.find(handle);
		if (it == timers_.end()) return false;
		auto& timer = it->second;
		
		timer.delay = ToDuration(new_delay);
		if (restart) timer.end = world_.GetTime() + timer.delay;
		return true;
	}

	bool TimerManager::RemoveTimer(TimerHandle handle) noexcept
	{
		return timers_.erase(handle);
	}

	bool TimerManager::IsTimerExists(TimerHandle handle) const noexcept
	{
		return timers_.contains(handle) || pending_timers_.contains(handle);
	}

	float TimerManager::TimeLeft(TimerHandle handle) const noexcept
	{
		const auto timer = timers_.find(handle);
		if (timer == timers_.end()) return 0;
		return ToFloat(timer->second.end - world_.GetTime());
	}

	TimerHandle TimerHandle::Create() noexcept
	{
		static size_t key = 0;
		TimerHandle handle;
		handle.key = ++key;
		return handle;
	}

	TimerManager::TimerManager(CWorld& world) noexcept:
		world_{world}
	{
	}

	TimerManager::~TimerManager() = default;

	void TimerManager::Update()
	{
		timers_.merge(pending_timers_);
		
		const auto cur = world_.GetTime();
		for (auto it = timers_.begin(); it != timers_.end();)
		{
			auto& [handle, timer] = *it;
			if (timer.end <= cur)
			{
				const auto loop = timer.fn();
				if (loop == Loop::kContinue)
				{
					timer.end += timer.delay;
					++it;
				}
				else
				{
					it = timers_.erase(it);
				}
			}
			else
			{
				++it;
			}
		}
	}
}

size_t std::hash<oeng::TimerHandle>::operator()(oeng::TimerHandle key) const noexcept
{
	return key.key;
}
