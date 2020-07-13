#pragma once
#include "Name.hpp"
#include "Templates/Time.hpp"

namespace oeng
{
	struct ScopeCycleStat;
	using ScopeCycleStats = std::unordered_map<Name, ScopeCycleStat>;
	
	struct ScopeCycleStat
	{
		Duration duration{};
		uint64_t count{};
		ScopeCycleStats children;
	};

	class OEAPI ScopeCycleManager
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
		ScopeCycleStats stats_;
	};

	class OEAPI ScopeCycleCounter
	{
	public:
		OE_DELETE_CPMV(ScopeCycleCounter);
		
		explicit ScopeCycleCounter(Name name);
		~ScopeCycleCounter();
	};
}
