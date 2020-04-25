#include "TimerManager.h"
#include "World.h"

namespace oeng
{
	struct CTimerManager::FTimer
	{
		std::function<Loop()> fn;
		TimePoint end;
		Duration delay;
	};

	FTimerHandle CTimerManager::SetLoopTimer(Duration delay, std::function<Loop()>&& fn)
	{
		const auto handle = FTimerHandle::Create();
		pending_timers_.insert({handle, {std::move(fn), world_.GetTime() + delay, delay}});
		return handle;
	}

	FTimerHandle CTimerManager::SetTimer(Duration delay, std::function<void()>&& fn)
	{
		return SetLoopTimer(delay, [func = std::move(fn)]{func(); return Loop::kStop;});
	}

	FTimerHandle CTimerManager::SetLoopTimer(float delay_in_seconds, std::function<Loop()>&& fn)
	{
		return SetLoopTimer(duration<float>{delay_in_seconds}, std::move(fn));
	}

	FTimerHandle CTimerManager::SetTimer(float delay_in_seconds, std::function<void()>&& fn)
	{
		return SetTimer(duration<float>{delay_in_seconds}, std::move(fn));
	}

	void CTimerManager::SetTimerForNextTick(std::function<void()>&& fn)
	{
		SetTimer(Duration{}, std::move(fn));
	}

	bool CTimerManager::IsTimerExists(const FTimerHandle& handle) const noexcept
	{
		return timers_.contains(handle) || pending_timers_.contains(handle);
	}

	FTimerHandle FTimerHandle::Create() noexcept
	{
		static size_t key = 0;
		FTimerHandle handle;
		handle.key = ++key;
		return handle;
	}

	CTimerManager::CTimerManager(CWorld& world):
		world_{world}
	{
	}

	CTimerManager::~CTimerManager() = default;

	void CTimerManager::Update()
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

size_t std::hash<oeng::FTimerHandle>::operator()(const oeng::FTimerHandle& key) const noexcept
{
	return key.key;
}
