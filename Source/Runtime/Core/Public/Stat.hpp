#pragma once
#include "Name.hpp"
#include "Templates/Time.hpp"

namespace logcat
{
extern CORE_API const LogCategory kStat;
}

namespace oeng
{
inline namespace core
{

class CORE_API ScopeStackCounter
{
  public:
    explicit ScopeStackCounter(Name name);
    ~ScopeStackCounter();
};

class CORE_API ScopeCounter
{
  public:
    explicit ScopeCounter(Name name) noexcept : start_{Clock::now()}, name_{name}
    {
    }

    ~ScopeCounter();

  private:
    TimePoint start_;
    Name name_;
};

CORE_API void LogStats(uint64_t ticks);

} // namespace core
} // namespace oeng

#ifdef NDEBUG
#define SCOPE_COUNTER(name)
#define SCOPE_STACK_COUNTER(name)
#else
#define SCOPE_COUNTER(name)                                                                                            \
    ScopeCounter _##name##_scope_counter                                                                               \
    {                                                                                                                  \
        u8## #name##sv                                                                                                 \
    }
#define SCOPE_STACK_COUNTER(name)                                                                                      \
    ScopeStackCounter _##name##_scope_stack_counter                                                                    \
    {                                                                                                                  \
        u8## #name##sv                                                                                                 \
    }
#endif
