#include "World.hpp"
#include "Stat.hpp"
#include "TimerManager.hpp"
#include "Actors/Actor.hpp"
#include "Components/SphereComponent.hpp"

namespace oeng
{
inline namespace engine
{
World::World()
    : timer_{*this}, delta_seconds_{0}
{
}

World::~World() = default;

void World::BeginTick()
{
    time_ = Clock::now();
}

void World::Tick()
{
    SCOPE_STACK_COUNTER(WorldTick);
    UpdateTime();
    UpdateGame();
}

void World::AddCollision(SphereComponent& comp)
{
    collisions_.emplace_back(comp);
}

void World::RemoveCollision(SphereComponent& comp)
{
    auto pr = [&](const SphereComponent& v)
    {
        return &v == &comp;
    };
    const auto found = std::find_if(collisions_.crbegin(), collisions_.crend(), pr);
    if (found != collisions_.crend())
        collisions_.erase(found.base() - 1);
}

void World::UpdateGame()
{
    for (auto* actor : updated_actors_)
        actor->Update(delta_seconds_);

    for (size_t i = 0; i < collisions_.size(); ++i)
        for (auto j = i + 1; j < collisions_.size(); ++j)
            collisions_[i]->DoOverlap(*collisions_[j]);

    timer_.Update();

    // Use index-based loop because BeginPlay() can spawn new actors
    for (size_t i = 0; i < actors_.size(); ++i)
    {
        if (actors_[i]->can_ever_update_)
            updated_actors_.push_back(actors_[i].get());

        actors_[i]->BeginPlay();
    }

    for (auto it = actors_.rbegin(); it != actors_.rend();)
    {
        const auto& actor = **it;
        if (actor.IsPendingKill())
        {
            if (actor.can_ever_update_)
                updated_actors_.erase(std::find(updated_actors_.begin(), updated_actors_.end(), &actor));

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
}
}
