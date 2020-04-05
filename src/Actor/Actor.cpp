#include "Actor/Actor.h"
#include "Component/ActorComponent.h"
#include "MathUtil.h"
#include "World.h"

namespace game
{
	AActor::AActor(CWorld& world):
		pos_{}, scale_{1}, world_transform_{}, world_{world}
	{
	}

	AActor::~AActor() = default;

	void AActor::BeginPlay() const
	{
		for (auto& c : comps_)
			c->BeginPlay();
	}

	vec2 AActor::GetForward() const noexcept
	{
		return math::R2V(rot_);
	}

	void AActor::Update(const float delta_seconds)
	{
		UpdateLifespan(delta_seconds);
		UpdateComponents(delta_seconds);
		UpdateActor(delta_seconds);
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

	CEngine& AActor::GetEngine() const noexcept
	{
		return world_.GetEngine();
	}

	CTimerManager& AActor::GetTimerManager() const noexcept
	{
		return world_.GetTimerManager();
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

	void AActor::SetEnabled(bool enable)
	{
		for (auto& c : comps_)
		{
			c->SetEnabled(enable);
		}
	}

	void AActor::SetPos(const vec2& new_pos, bool recompute_world_transform) noexcept
	{
		pos_ = new_pos;
		if (recompute_world_transform) RecomputeWorldTransform();
	}

	void AActor::SetRot(Degrees new_rot, bool recompute_world_transform) noexcept
	{
		rot_ = new_rot;
		if (recompute_world_transform) RecomputeWorldTransform();
	}
	
	void AActor::SetScale(const vec2& scale, bool recompute_world_transform) noexcept
	{
		scale_ = scale;
		if (recompute_world_transform) RecomputeWorldTransform();
	}

	void AActor::RecomputeWorldTransform() noexcept
	{
		
	}
}
