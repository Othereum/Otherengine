#pragma once
#include "Name.hpp"
#include "Templates/Time.hpp"

namespace oeng::core
{
	struct ScopeStat
	{
		Duration duration{};
		uint64_t count{};
	};
	
	struct ScopeCycleStat : ScopeStat
	{
		std::map<Name, ScopeCycleStat> children;
	};

	class CORE_API ScopeCycleManager
	{
	public:
		void Push(Name name);
		void Pop();
		[[nodiscard]] auto& Stats() const noexcept { return stats_; }

	private:
		struct Frame
		{
			Name name;
			TimePoint start;
		};

		std::vector<Frame> frames_;
		std::map<Name, ScopeCycleStat> stats_;
	};

	class CORE_API ScopeCycleCounter
	{
	public:
		DELETE_CPMV(ScopeCycleCounter);
		
		explicit ScopeCycleCounter(Name name);
		~ScopeCycleCounter();
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
}
