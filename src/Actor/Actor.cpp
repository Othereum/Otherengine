#include "Actor/Actor.h"
#include "Component/ActorComponent.h"
#include "MathUtil.h"

namespace Game
{
	AActor::AActor(CEngine& engine)
		:engine_{engine}
	{
	}

	AActor::~AActor() = default;

	void AActor::BeginPlay() const
	{
		for (auto& c : comps_)
			c->BeginPlay();
	}

	TFVector2 AActor::GetForward() const noexcept
	{
		return Math::R2V(rot_);
	}

	void AActor::Update(const float deltaSeconds)
	{
		UpdateLifespan(deltaSeconds);
		UpdateComponents(deltaSeconds);
		UpdateActor(deltaSeconds);
	}

	void AActor::UpdateComponents(const float delta_seconds)
	{
		for (const auto& comp : comps_)
		{
			comp->Update(delta_seconds);
		}
	}

	void AActor::UpdateLifespan(const float delta_seconds)
	{
		if (lifespan_ > 0 && (lifespan_ -= delta_seconds) <= 0)
		{
			Destroy();
		}
	}

	void AActor::RegisterComponent(std::unique_ptr<CActorComponent>&& comp)
	{
		auto cmp = [](const std::unique_ptr<CActorComponent>& a, const std::unique_ptr<CActorComponent>& b)
		{
			return a->GetUpdateOrder() <= b->GetUpdateOrder();
		};
		const auto pos = std::lower_bound(comps_.begin(), comps_.end(), comp, cmp);
		comps_.insert(pos, std::move(comp));
	}

	void AActor::Destroy()
	{
		state_ = EState::dead;
	}
}
