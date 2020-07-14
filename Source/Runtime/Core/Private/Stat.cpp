#include "Stat.hpp"
#include "Debug.hpp"
#include "Templates/Time.hpp"

namespace oeng
{
	OE_EXPORT ScopeCycleManager scope_cycle_manager;

	void ScopeCycleManager::Push(Name name)
	{
		frames_.push_back({name, Clock::now()});
	}

	void ScopeCycleManager::Pop()
	{
		const auto end = Clock::now();
		const auto top = frames_.back();
		frames_.pop_back();
		
		std::reference_wrapper<std::map<Name, ScopeCycleStat>> stats = stats_;
		for (auto& frame : frames_)
			stats = stats.get()[frame.name].children;
		
		auto& stat = stats.get()[top.name];
		stat.duration += end - top.start;
		++stat.count;
	}

	ScopeCycleCounter::ScopeCycleCounter(Name name)
	{
		scope_cycle_manager.Push(name);
	}

	ScopeCycleCounter::~ScopeCycleCounter()
	{
		try
		{
			scope_cycle_manager.Pop();
		}
		catch (const std::exception& e)
		{
			OE_ELOG(u8"ScopeCycleCounter: {}", What(e));
		}
	}

	OE_EXPORT std::unordered_map<Name, ScopeStat> scope_stats;

	ScopeCounter::~ScopeCounter()
	{
		try
		{
			const auto end = Clock::now();
			auto& counter = scope_stats[name_];
			counter.duration += end - start_;
			++counter.count;
		}
		catch (const std::exception& e)
		{
			OE_ELOG(u8"ScopeCounter: {}", What(e));
		}
	}
}
