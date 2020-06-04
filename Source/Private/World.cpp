#include "World.hpp"
#include "Actor.hpp"
#include "Components/CircleComponent.hpp"
#include "TimerManager.hpp"

namespace oeng
{
	World::World(Engine& engine)
		:engine_{engine}, timer_{*this}, delta_seconds_{0}
	{
	}

	World::~World() = default;

	void World::Tick()
	{
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
		
		for (auto&& pending : pending_actors_)
		{
			auto& actor = *pending;
			actors_.push_back(std::move(pending));
			actor.BeginPlay();
		}
		pending_actors_.clear();

		for (auto it = actors_.rbegin(); it != actors_.rend();)
		{
			const auto& actor = **it;
			if (actor.IsPendingKill())
			{
				const auto next = actors_.erase(it.base() - 1);
				it = std::make_reverse_iterator(next);
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
		delta_seconds_ = std::chrono::duration<Float>{now - time_}.count();
		time_ = now;
	}

	void World::RegisterActor(SharedPtr<AActor>&& actor)
	{
		pending_actors_.push_back(std::move(actor));
	}
}
