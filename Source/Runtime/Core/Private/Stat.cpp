#include "Stat.hpp"
#include <unordered_map>
#include <vector>
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
		
		std::reference_wrapper<ScopeCycleStats> stats = stats_;
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
			OE_ELOG(What(e));
		}
	}
}
