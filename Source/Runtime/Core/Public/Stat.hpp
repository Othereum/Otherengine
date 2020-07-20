#pragma once
#include "Name.hpp"
#include "Templates/HashMap.hpp"
#include "Templates/Time.hpp"

namespace oeng::core
{
	struct ScopeStat
	{
		Duration duration{};
		uint64_t count{};
	};
	
	struct ScopeStackStat : ScopeStat
	{
		TreeMap<Name, ScopeStackStat> children;
	};

	class CORE_API ScopeStackCounter
	{
	public:
		DELETE_CPMV(ScopeStackCounter);
		
		explicit ScopeStackCounter(Name name);
		~ScopeStackCounter();
	};
	
	class CORE_API ScopeCounter
	{
	public:
		DELETE_CPMV(ScopeCounter);
		
		explicit ScopeCounter(Name name) noexcept
			:start_{Clock::now()}, name_{name}
		{
		}
		
		~ScopeCounter();

	private:
		TimePoint start_;
		Name name_;
	};
	
	class CounterManager
	{
		friend ScopeCounter;
		friend ScopeStackCounter;
		friend class EngineBase;
		
		struct Frame
		{
			Name name;
			TimePoint start;
		};

		CounterManager() = default;
		
		void PushScope(Name name);
		void PopScope();
		
		HashMap<Name, ScopeStat> scope_stats_;
		TreeMap<Name, ScopeStackStat> scope_stack_stats_;
		DyArr<Frame> frames_;
	};
}
