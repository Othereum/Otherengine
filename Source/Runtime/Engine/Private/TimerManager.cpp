#include "TimerManager.hpp"
#include "World.hpp"

namespace oeng
{
inline namespace engine
{
[[nodiscard]] constexpr Clock::duration ToDuration(Float sec) noexcept
{
    return duration_cast<Clock::duration>(time::duration<Float>{sec});
}

[[nodiscard]] constexpr Float ToFloat(Clock::duration dur) noexcept
{
    return duration_cast<time::duration<Float>>(dur).count();
}

TimerHandle TimerManager::SetTimer(Float delay_in_seconds, std::function<void()>&& callback)
{
    return SetLoopTimer(delay_in_seconds, [fn = std::move(callback)]
    {
        fn();
        return Loop::kStop;
    });
}

TimerHandle TimerManager::SetLoopTimer(Float delay_in_seconds, std::function<Loop()>&& callback)
{
    TimerHandle handle;
    handle.id_ = ++timer_id_;

    const auto delay = ToDuration(delay_in_seconds);
    pending_timers_.try_emplace(handle.id_, FTimer{std::move(callback), world_.GetTime() + delay, delay});

    return handle;
}

bool TimerManager::UpdateTimer(TimerHandle handle, Float new_delay, bool restart) noexcept
{
    const auto it = timers_.find(handle.id_);
    if (it == timers_.end())
        return false;

    auto& timer = it->second;
    timer.delay = ToDuration(new_delay);

    if (restart)
        timer.end = world_.GetTime() + timer.delay;

    return true;
}

bool TimerManager::RemoveTimer(TimerHandle handle) noexcept
{
    return timers_.erase(handle.id_);
}

bool TimerManager::IsTimerExists(TimerHandle handle) const noexcept
{
    return timers_.contains(handle.id_) || pending_timers_.contains(handle.id_);
}

Float TimerManager::TimeLeft(TimerHandle handle) const noexcept
{
    const auto timer = timers_.find(handle.id_);

    if (timer == timers_.end())
        return 0;

    return ToFloat(timer->second.end - world_.GetTime());
}

void TimerManager::Update()
{
    timers_.merge(pending_timers_);

    const auto cur = world_.GetTime();
    for (auto it = timers_.begin(); it != timers_.end();)
    {
        auto& [handle, timer] = *it;
        if (timer.end <= cur)
        {
            const auto loop = timer.fn();
            if (loop == Loop::kContinue)
            {
                timer.end += timer.delay;
                ++it;
            }
            else
            {
                it = timers_.erase(it);
            }
        }
        else
        {
            ++it;
        }
    }
}
}
}
