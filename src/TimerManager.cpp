#include "TimerManager.h"
#include "World.h"

namespace Game
{
	struct CTimerManager::FTimer
	{
		std::function<bool()> fn;
		TimePoint end;
		Duration delay;
		bool loop;
	};

	FTimerHandle CTimerManager::SetTimer(Duration delay, bool loop, std::function<bool()>&& fn)
	{
		const auto handle = FTimerHandle::Create();
		timers_.insert({handle, {std::move(fn), world_.GetTime() + delay, delay, loop}});
		return handle;
	}

	FTimerHandle FTimerHandle::Create() noexcept
	{
		static auto key = 0;
		FTimerHandle handle;
		handle.key = key++;
		return handle;
	}

	CTimerManager::CTimerManager(CWorld& world):
		world_{world}
	{
	}
}

size_t std::hash<Game::FTimerHandle>::operator()(const Game::FTimerHandle& key) const noexcept
{
	return size_t(key.key);
}
