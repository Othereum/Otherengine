#pragma once
#include <functional>
#include <unordered_map>
#include "TimeUtil.h"

namespace oeng
{
	struct FTimerHandle;
}

template <>
struct std::hash<oeng::FTimerHandle>
{
	size_t operator()(const oeng::FTimerHandle& key) const noexcept;
};

namespace oeng
{
	enum class Loop
	{
		kStop, kContinue
	};
	
	struct FTimerHandle
	{
		constexpr bool operator==(const FTimerHandle&) const noexcept = default;
		
	private:
		friend class CTimerManager;
		friend std::hash<FTimerHandle>;
		
		static FTimerHandle Create() noexcept;
		
		size_t key = 0;
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
		
		FTimerHandle SetLoopTimer(float delay_in_seconds, std::function<Loop()>&& fn = DefLoopFn);
		FTimerHandle SetTimer(float delay_in_seconds, std::function<void()>&& fn = DefFn);
		
		void SetTimerForNextTick(std::function<void()>&& fn);
		[[nodiscard]] bool IsTimerExists(const FTimerHandle& handle) const noexcept;

		CTimerManager(const CTimerManager&) = delete;
		CTimerManager(CTimerManager&&) = delete;
		CTimerManager& operator=(const CTimerManager&) = delete;
		CTimerManager& operator=(CTimerManager&&) = delete;

	private:
		static Loop DefLoopFn() noexcept { return Loop::kStop; }
		static void DefFn() noexcept {}
		
		struct FTimer;
		
		CWorld& world_;
		std::unordered_map<FTimerHandle, FTimer> timers_;
		std::unordered_map<FTimerHandle, FTimer> pending_timers_;
	};
}
