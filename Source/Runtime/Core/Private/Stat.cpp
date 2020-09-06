#include "Stat.hpp"

namespace logcat
{
const LogCategory kStat{u8"Stat"sv};
}

namespace oeng
{
inline namespace core
{
struct StatData
{
    struct ScopeStat
    {
        Duration duration{};
        uint64_t count{};
    };

    struct ScopeStackStat : ScopeStat
    {
        std::map<Name, ScopeStackStat> children;
    };

    struct ScopeFrame
    {
        Name name;
        TimePoint start;
    };

    [[nodiscard]] static StatData& Get()
    {
        static StatData data;
        return data;
    }

    void PushScope(Name name)
    {
        scope_frames.push_back({name, Clock::now()});
    }

    void PopScope()
    {
        const auto end = Clock::now();
        const auto top = scope_frames.back();
        scope_frames.pop_back();

        auto* stats = &scope_stack_stats;
        for (auto& frame : scope_frames)
            stats = &(*stats)[frame.name].children;

        auto& stat = (*stats)[top.name];
        stat.duration += end - top.start;
        ++stat.count;
    }

    void LogStat(uint64_t ticks, Name name, const ScopeStat& stat, int depth = 0)
    {
        const auto time = duration_cast<time::duration<Float, std::milli>>(stat.duration / ticks).count();
        const auto count = ToFloat(stat.count) / ToFloat(ticks);
        OE_LOG(kStat, kLog, u8"[Stat]{:^{}} {} took {:.2f} ms, {:.1f} times"sv, u8""sv, depth, *name, time, count);
    }

    void LogStats(uint64_t ticks, const std::map<Name, ScopeStackStat>& stats, int depth = 0)
    {
        for (const auto& [name, stat] : stats)
        {
            LogStat(ticks, name, stat, depth);
            LogStats(ticks, stat.children, depth + 1);
        }
    }

    void LogStats(uint64_t ticks)
    {
        if (ticks == 0)
            return;

        LogStats(ticks, scope_stack_stats);

        for (auto& [name, stat] : scope_stats)
        {
            LogStat(ticks, name, stat);
        }
    }

    std::unordered_map<Name, ScopeStat> scope_stats;
    std::map<Name, ScopeStackStat> scope_stack_stats;
    std::vector<ScopeFrame> scope_frames;
};

void LogStats(uint64_t ticks)
{
    StatData::Get().LogStats(ticks);
}

ScopeStackCounter::ScopeStackCounter(Name name)
{
    StatData::Get().PushScope(name);
}

ScopeStackCounter::~ScopeStackCounter()
{
    StatData::Get().PopScope();
}

ScopeCounter::~ScopeCounter()
{
    const auto end = Clock::now();
    auto& counter = StatData::Get().scope_stats[name_];
    counter.duration += end - start_;
    ++counter.count;
}
}
}
