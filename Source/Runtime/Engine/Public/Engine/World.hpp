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

    template <class T> T& SpawnActor()
    {
    }

    AActor& SpawnActor(Name class_name);

    void AddCollision(SphereComponent& comp);
    void RemoveCollision(SphereComponent& comp);

    [[nodiscard]] auto& GetActors() const noexcept
    {
        return actors_;
    }

    [[nodiscard]] TimePoint GetTime() const noexcept
    {
        return time_;
    }

    [[nodiscard]] Float GetDeltaSeconds() const noexcept
    {
        return delta_seconds_;
    }

    TimerManager timer_manager;

  private:
    void UpdateGame();
    void UpdateTime();

    std::vector<SphereComponent*> collisions_;
    std::vector<std::shared_ptr<AActor>> actors_;
    std::vector<std::shared_ptr<AActor>> pending_actors_;

    TimePoint time_;
    Float delta_seconds_;
};

} // namespace engine
} // namespace oeng
