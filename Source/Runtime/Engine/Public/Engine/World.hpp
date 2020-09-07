#pragma once
#include "TimerManager.hpp"

namespace oeng
{
inline namespace engine
{
class AActor;
class SphereComponent;

class ENGINE_API World
{
public:
    DELETE_CPMV(World);

    World();
    ~World();

    void BeginTick();
    void Tick();

    template <class T>
    T& SpawnActor()
    {
        auto ptr = std::make_shared<T>();
        auto& ref = *ptr;
        ref.world_ = this;
        pending_actors_.emplace_back(std::move(ptr));
        return ref;
    }

    template <std::invocable<const std::shared_ptr<AActor>&> Fn>
    void ForEachActor(Fn&& fn) const
    {
        for (auto& actor : actors_)
        {
            fn(actor);
        }
    }

    void AddCollision(SphereComponent& comp);
    void RemoveCollision(SphereComponent& comp);

    [[nodiscard]] TimerManager& GetTimerManager() noexcept
    {
        return timer_;
    }

    [[nodiscard]] TimePoint GetTime() const noexcept
    {
        return time_;
    }

    [[nodiscard]] Float GetDeltaSeconds() const noexcept
    {
        return delta_seconds_;
    }

private:
    void UpdateGame();
    void UpdateTime();

    TimerManager timer_;

    std::vector<SphereComponent*> collisions_;
    std::vector<std::shared_ptr<AActor>> actors_;
    std::vector<std::shared_ptr<AActor>> pending_actors_;

    TimePoint time_;
    Float delta_seconds_;
};
}
}
