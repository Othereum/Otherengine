#include "Stat.hpp"
#include "Debug.hpp"
#include "EngineBase.hpp"
#include "otm/Basic.hpp"
#include "Templates/Time.hpp"

namespace oeng::core
{
	static void LogStat(Name name, const ScopeStat& stat, int depth = 0)
	{
		const auto ticks = EngineBase::Get().GetTickCount();
		const auto time = duration_cast<time::duration<Float, std::milli>>(stat.duration / ticks).count();
		const auto count = ToFloat(stat.count) / ToFloat(ticks);
		log::Debug(u8"[Stat]{:^{}} {} took {:.2f} ms, {:.1f} times"sv, u8""sv, depth, *name, time, count);
	}

	static void LogStats(const TreeMap<Name, ScopeStackStat>& stats, int depth = 0)
	{
		for (auto& [name, stat] : stats)
		{
			LogStat(name, stat, depth);
			LogStats(stat.children, depth + 1);
		}
	}
	
	CounterManager::~CounterManager()
	{
		auto& engine = EngineBase::Get();
		if (engine.GetTickCount() == 0) return;
		
		LogStats(engine.counters_.scope_stack_stats_);

		for (auto& [name, stat] : engine.counters_.scope_stats_)
		{
			LogStat(name, stat);
		}
	}

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
		EngineBase::Get().counters_.PushScope(name);
	}

	ScopeStackCounter::~ScopeStackCounter()
	{
		try
		{
			EngineBase::Get().counters_.PopScope();
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
			auto& counter = EngineBase::Get().counters_.scope_stats_[name_];
			counter.duration += end - start_;
			++counter.count;
		}
		catch (const std::exception& e)
		{
			OE_ELOG(u8"ScopeCounter: {}"sv, What(e));
		}
	}
}
