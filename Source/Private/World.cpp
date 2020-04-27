#include "World.h"
#include "Actor.h"
#include "Components/CircleComponent.h"
#include "TimerManager.h"

namespace oeng
{
	CWorld::CWorld(CEngine& engine):
		engine_{engine},
		timer_{std::make_unique<TimerManager>(*this)},
		delta_seconds_{0}
	{
	}

	CWorld::~CWorld() = default;

	void CWorld::Tick()
	{
		UpdateTime();
		UpdateGame();
	}

	void CWorld::RegisterCollision(CCircleComponent& comp)
	{
		collisions_.emplace_back(comp);
	}

	void CWorld::UnregisterCollision(CCircleComponent& comp)
	{
		auto pr = [&](const CCircleComponent& v) { return &v == &comp; };
		const auto found = std::find_if(collisions_.crbegin(), collisions_.crend(), pr);
		if (found != collisions_.crend()) collisions_.erase(found.base() - 1);
	}

	void CWorld::UpdateGame()
	{
		for (const auto& actor : actors_)
			actor->Update(delta_seconds_);
		
		for (size_t i = 0; i < collisions_.size(); ++i)
			for (auto j = i+1; j < collisions_.size(); ++j)
				collisions_[i].get().TestOverlap(collisions_[j]);

		timer_->Update();
		
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

	void CWorld::UpdateTime()
{
		const auto now = Clock::now();
		delta_seconds_ = std::chrono::duration<float>{now - time_}.count();
		time_ = now;
	}

	void CWorld::RegisterActor(std::shared_ptr<AActor> actor)
	{
		pending_actors_.push_back(std::move(actor));
	}
}
