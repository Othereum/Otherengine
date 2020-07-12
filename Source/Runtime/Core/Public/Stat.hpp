#pragma once
#include "Core.hpp"
#include "Templates/Time.hpp"

namespace oeng::detail
{
	class OEAPI StopWatch
	{
	public:
		explicit StopWatch(std::u8string_view name);
		
		void Start() noexcept
		{
			start_ = Clock::now();
		}
		
		void Stop() noexcept
		{
			duration_ += Clock::now() - start_;
			++count_;
		}

		[[nodiscard]] std::u8string_view Name() const noexcept
		{
			return name_;
		}
		
		[[nodiscard]] Duration Average() const noexcept
		{
			if (count_ == 0) return {};
			return duration_ / count_;
		}

	private:
		std::u8string_view name_;
		TimePoint start_{};
		Duration duration_{};
		uint64_t count_{};
	};

	class ScopeTimer
	{
	public:
		OE_DELETE_CPMV(ScopeTimer);
		
		explicit ScopeTimer(StopWatch& watch) noexcept
			:watch_{watch}
		{
			watch.Start();
		}

		~ScopeTimer()
		{
			watch_.Stop();
		}

	private:
		StopWatch& watch_;
	};
}

#define OE_STAT_SCOPE_TIMER(name) static ::oeng::detail::StopWatch _##name##StopWatch{u8## #name}; ::oeng::detail::ScopeTimer _##name##Timer{_##name##StopWatch}
