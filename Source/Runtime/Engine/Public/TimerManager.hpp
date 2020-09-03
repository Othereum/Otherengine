#pragma once

namespace oeng
{
inline namespace engine
{
enum class Loop
{
    kStop,
    kContinue
};

struct TimerHandle
{
    constexpr bool operator==(const TimerHandle&) const noexcept = default;

private:
    friend class TimerManager;
    size_t id_ = 0;
};

class ENGINE_API TimerManager
{
public:
    explicit TimerManager(class World& world) noexcept
        : world_{world}
    {
    }

    void Update();

    TimerHandle SetTimer(Float delay_in_seconds, std::function<void()>&& callback);
    TimerHandle SetLoopTimer(Float delay_in_seconds, std::function<Loop()>&& callback);

    /**
     * Update timer
     * @param handle Handle of timer to be updated
     * @param new_delay New delay in seconds
     * @param restart Should the timer be restarted based on the current time?
     * @return `true` if the timer exists and updated successfully
     */
    bool UpdateTimer(TimerHandle handle, Float new_delay, bool restart = true) noexcept;

    /**
     * Remove timer
     * @param handle Handle of timer
     * @return If the timer existed and removed successfully
     */
    bool RemoveTimer(TimerHandle handle) noexcept;

    /**
     * Get remaining time of timer
     * @param handle Handle of timer
     * @return Remaining time. If timer not exists, returns 0
     */
    [[nodiscard]] Float TimeLeft(TimerHandle handle) const noexcept;
    [[nodiscard]] bool IsTimerExists(TimerHandle handle) const noexcept;

private:
    struct FTimer
    {
        std::function<Loop()> fn;
        Clock::time_point end;
        Clock::duration delay;
    };

    World& world_;
    size_t timer_id_ = 0;
    std::unordered_map<size_t, FTimer> timers_;
    std::unordered_map<size_t, FTimer> pending_timers_;
};
}
}
