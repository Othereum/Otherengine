#pragma once
#include "Name.hpp"
#include "Templates/Time.hpp"

namespace logcat
{
extern CORE_API const LogCategory kStat;
}

namespace oeng::core
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
        : start_{Clock::now()}, name_{name}
    {
    }

    ~ScopeCounter();

private:
    TimePoint start_;
    Name name_;
};

class CORE_API CounterManager
{
    DELETE_CPMV(CounterManager);

    friend ScopeCounter;
    friend ScopeStackCounter;
    friend class EngineBase;

    struct Frame
    {
        Name name;
        TimePoint start;
    };

    CounterManager() = default;
    ~CounterManager();

    void PushScope(Name name);
    void PopScope();

    std::unordered_map<Name, ScopeStat> scope_stats_;
    std::map<Name, ScopeStackStat> scope_stack_stats_;
    std::vector<Frame> frames_;
};
}

#ifdef NDEBUG
#define SCOPE_COUNTER(name)
#define SCOPE_STACK_COUNTER(name)
#else
	#define SCOPE_COUNTER(name) ScopeCounter _##name##_scope_counter{u8 ## #name ## sv}
	#define SCOPE_STACK_COUNTER(name) ScopeStackCounter _##name##_scope_stack_counter{u8 ## #name ## sv}
#endif
