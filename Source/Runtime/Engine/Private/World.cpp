#include "World.hpp"
#include "Stat.hpp"
#include "TimerManager.hpp"
#include "Actors/Actor.hpp"
#include "Components/CircleComponent.hpp"

namespace oeng
{
	World::World(Engine& engine)
		:engine_{engine}, timer_{*this}, delta_seconds_{0}
	{
	}

	World::~World() = default;

	void World::BeginTick()
	{
		time_ = Clock::now();
	}

	void World::Tick()
	{
		ScopeCycleCounter counter{u8"WorldTick"};
		UpdateTime();
		UpdateGame();
	}

	void World::RegisterCollision(CircleComponent& comp)
	{
		collisions_.emplace_back(comp);
	}

	void World::UnregisterCollision(CircleComponent& comp)
	{
		auto pr = [&](const CircleComponent& v) { return &v == &comp; };
		const auto found = std::find_if(collisions_.crbegin(), collisions_.crend(), pr);
		if (found != collisions_.crend()) collisions_.erase(found.base() - 1);
	}

	void World::UpdateGame()
	{
		for (const auto& actor : actors_)
			actor->Update(delta_seconds_);
		
		for (size_t i = 0; i < collisions_.size(); ++i)
			for (auto j = i+1; j < collisions_.size(); ++j)
				collisions_[i].get().TestOverlap(collisions_[j]);

		timer_.Update();

		// Use index-based loop because BeginPlay() can spawn new actors
		for (size_t i=0; i<pending_actors_.size(); ++i)  // NOLINT(modernize-loop-convert)
		{
			auto& actor = *pending_actors_[i];
			actors_.push_back(std::move(pending_actors_[i]));
			actor.BeginPlay();
		}
		pending_actors_.clear();

		for (auto it = actors_.rbegin(); it != actors_.rend();)
		{
			const auto& actor = **it;
			if (actor.IsPendingKill())
			{
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
