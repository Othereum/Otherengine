#pragma once
#include <functional>
#include <unordered_map>
#include "TimeUtil.h"

namespace game
{
	struct FTimerHandle;
}

template <>
struct std::hash<game::FTimerHandle>
{
	size_t operator()(const game::FTimerHandle& key) const noexcept;
};

namespace game
{
	enum class Loop
	{
		kStop, kContinue
	};
	
	struct FTimerHandle
	{
		bool operator==(const FTimerHandle&) const noexcept = default;
		bool operator!=(const FTimerHandle&) const noexcept = default;
		
	private:
		friend class CTimerManager;
		friend std::hash<FTimerHandle>;
		
		static FTimerHandle Create() noexcept;
		
		int key = -1;
	};

	class CTimerManager
	{
	public:
		explicit CTimerManager(class CWorld& world);
		~CTimerManager();
		
		void Update();

		template <class R, class P>
		FTimerHandle SetLoopTimer(duration<R, P> delay, std::function<Loop()>&& fn = DefLoopFn)
		{
			return SetLoopTimer(duration_cast<Duration>(delay), std::move(fn));
		}
		
		template <class R, class P>
		FTimerHandle SetTimer(duration<R, P> delay, std::function<void()>&& fn = DefFn)
		{
			return SetTimer(duration_cast<Duration>(delay), std::move(fn));
		}
		
		FTimerHandle SetLoopTimer(Duration delay, std::function<Loop()>&& fn = DefLoopFn);
		FTimerHandle SetTimer(Duration delay, std::function<void()>&& fn = DefFn);
		void SetTimerForNextTick(std::function<void()>&& fn);
		[[nodiscard]] bool IsTimerExists(const FTimerHandle& handle) const noexcept;

	private:
		static Loop DefLoopFn() noexcept { return Loop::kStop; }
		static void DefFn() noexcept {}
		
		struct FTimer;
		
		CWorld& world_;
		std::unordered_map<FTimerHandle, FTimer> timers_;
		std::unordered_map<FTimerHandle, FTimer> pending_timers_;
	};
}
