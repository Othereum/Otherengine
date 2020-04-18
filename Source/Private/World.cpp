#include "World.h"

#include "Actor/Actor.h"
#include "Component/CircleComponent.h"
#include "Component/InputComponent.h"
#include "Graphics/Renderer.h"
#include "TimerManager.h"

namespace oeng
{
	CWorld::CWorld(CEngine& engine):
		engine_{engine},
		renderer_{std::make_unique<graphics::CRenderer>()},
		timer_{std::make_unique<CTimerManager>(*this)}
	{
	}

	CWorld::~CWorld() = default;

	void CWorld::Tick()
	{
		UpdateGame();
		renderer_->DrawScene();
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
		const auto deltaSeconds = UpdateTime();
		
		for (const auto& actor : actors_)
			actor->Update(deltaSeconds);
		
		for (size_t i = 0; i < collisions_.size(); ++i)
			for (auto j = i+1; j < collisions_.size(); ++j)
				collisions_[i].get().TestOverlap(collisions_[j]);

		timer_->Update();
		
		for (auto&& pending : pendingActors_)
		{
			auto& actor = *pending;
			actors_.push_back(std::move(pending));
			actor.BeginPlay();
		}
		pendingActors_.clear();

		for (auto it = actors_.rbegin(); it != actors_.rend();)
		{
			const auto& actor = **it;
			if (actor.GetState() == AActor::EState::dead)
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

	float CWorld::UpdateTime()
{
		using namespace std::chrono;
		const auto now = steady_clock::now();
		const auto delta_seconds = duration<float>{now - time_}.count();
		time_ = now;
		return delta_seconds;
	}

	void CWorld::RegisterActor(std::unique_ptr<AActor>&& actor)
	{
		pendingActors_.push_back(std::move(actor));
	}
}
