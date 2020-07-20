#include "Stat.hpp"
#include "Debug.hpp"
#include "EngineBase.hpp"
#include "Templates/Time.hpp"

#define COUNTERS (assert(kEngineBase), kEngineBase->counters_)

namespace oeng::core
{
	void CounterManager::PushScope(Name name)
	{
		frames_.push_back({name, Clock::now()});
	}

	void CounterManager::PopScope()
	{
		const auto end = Clock::now();
		const auto top = frames_.back();
		frames_.pop_back();
		
		std::reference_wrapper<TreeMap<Name, ScopeStackStat>> stats = scope_stack_stats_;
		for (auto& frame : frames_)
			stats = stats.get()[frame.name].children;
		
		auto& stat = stats.get()[top.name];
		stat.duration += end - top.start;
		++stat.count;
	}

	ScopeStackCounter::ScopeStackCounter(Name name)
	{
		COUNTERS.PushScope(name);
	}

	ScopeStackCounter::~ScopeStackCounter()
	{
		try
		{
			COUNTERS.PopScope();
		}
		catch (const std::exception& e)
		{
			OE_ELOG(u8"ScopeStackCounter::~ScopeStackCounter(): {}"sv, What(e));
		}
	}

	ScopeCounter::~ScopeCounter()
	{
		try
		{
			const auto end = Clock::now();
			auto& counter = COUNTERS.scope_stats_[name_];
			counter.duration += end - start_;
			++counter.count;
		}
		catch (const std::exception& e)
		{
			OE_ELOG(u8"ScopeCounter: {}"sv, What(e));
		}
	}
}
