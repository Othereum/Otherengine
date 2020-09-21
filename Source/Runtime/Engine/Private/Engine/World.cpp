#include "Engine/World.hpp"
#include "Components/SphereComponent.hpp"
#include "ConfigSystem.hpp"
#include "Engine/GameInstance.hpp"
#include "GameFramework/Actor.hpp"
#include "GameFramework/GameModeBase.hpp"
#include "Stat.hpp"
#include "TimerManager.hpp"

namespace logcat
{
const LogCategory kWorld{u8"World"sv};
}

namespace oeng
{
inline namespace engine
{

void World::BeginPlay()
{
    begun_play_ = true;

    auto& cfg = ConfigSystem::Get()(u8"Engine"sv);
    const auto gm_class = cfg.at("GameMapsSettings"s).at("GlobalDefaultGameMode"s).get<Name>();

    try
    {
        gamemode_ = &SpawnActor<AGameModeBase>(gm_class);
    }
    catch (const std::exception& e)
    {
        OE_LOG(kWorld, kErr, u8"Failed to create gamemode '{}': {}"sv, *gm_class, AsString8(e.what()));
        OE_LOG(kWorld, kErr, u8"Falling back to default gamemode..."sv);
        gamemode_ = &SpawnActor<AGameModeBase>();
    }

    time_ = Clock::now();
}

void World::Tick()
{
    SCOPE_STACK_COUNTER(WorldTick);
    UpdateTime();
    UpdateGame();
}

AActor& World::SpawnActor(Name class_name)
{
    auto ptr = NewObject<AActor>(class_name);
    auto& ref = *ptr;
    ref.world_ = this;
    pending_actors_.emplace_back(std::move(ptr));
    return ref;
}

void World::AddCollision(SphereComponent& comp)
{
    collisions_.emplace_back(comp);
}

void World::RemoveCollision(SphereComponent& comp)
{
    auto pr = [&](const SphereComponent& v) { return &v == &comp; };
    const auto found = std::find_if(collisions_.crbegin(), collisions_.crend(), pr);
    if (found != collisions_.crend())
        collisions_.erase(found.base() - 1);
}

Engine& World::GetEngine() const noexcept
{
    return game_instance.engine;
}

void World::UpdateGame()
{
    for (const auto& actor : actors_)
        actor->Update(delta_seconds_);

    for (size_t i = 0; i < collisions_.size(); ++i)
        for (auto j = i + 1; j < collisions_.size(); ++j)
            collisions_[i].get().DoOverlap(collisions_[j]);

    timer_manager.Update();

    // Use index-based loop because BeginPlay() can spawn new actors
    for (size_t i = 0; i < pending_actors_.size(); ++i)
    {
        auto& ref = *actors_[i];
        actors_.push_back(std::move(actors_[i]));
        ref.BeginPlay();
    }

    for (auto it = actors_.rbegin(); it != actors_.rend();)
    {
        auto& actor = **it;
        if (actor.IsPendingKill())
        {
            actor.EndPlay();
            const auto next = actors_.erase(it.base() - 1);
            it = make_reverse_iterator(next);
        }
        else
        {
            ++it;
        }
    }
}

void World::UpdateTime()
{
    const auto now = Clock::now();
    delta_seconds_ = time::duration<Float>{now - time_}.count();
    delta_seconds_ = Min(1_f, delta_seconds_);
    time_ = now;
}

} // namespace engine
} // namespace oeng
