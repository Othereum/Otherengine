#pragma once
#include "TimerManager.hpp"

namespace logcat
{
extern ENGINE_API const LogCategory kWorld;
}

namespace oeng
{
inline namespace engine
{

class AActor;
class SphereComponent;
class AGameModeBase;

class ENGINE_API World
{
  public:
    AActor& SpawnActor(Name class_name);

    template <class T>[[nodiscard]] T& SpawnActor(Name class_name)
    {
        return dynamic_cast<T&>(SpawnActor(class_name));
    }

    template <class T>[[nodiscard]] T& SpawnActor()
    {
        return SpawnActor<T>(T::class_name);
    }

    void AddCollision(SphereComponent& comp);
    void RemoveCollision(SphereComponent& comp);

    [[nodiscard]] AGameModeBase* GetGameMode() const noexcept
    {
        return gamemode_;
    }

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

    TimerManager timer_manager{*this};

  private:
    friend class Engine;
    void BeginPlay();
    void Tick();

    void UpdateGame();
    void UpdateTime();

    bool begun_play_ = false;
    TimePoint time_{};
    Float delta_seconds_ = 0;

    std::vector<SphereComponent*> collisions_;
    std::vector<std::shared_ptr<AActor>> actors_;
    std::vector<std::shared_ptr<AActor>> pending_actors_;

    AGameModeBase* gamemode_ = nullptr;
};

} // namespace engine
} // namespace oeng
