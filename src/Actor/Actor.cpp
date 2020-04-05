#include "Actor/Actor.h"
#include "Component/ActorComponent.h"
#include "MathUtil.h"
#include "World.h"

namespace game
{
	AActor::AActor(CWorld& world)
		:world_{world}
	{
	}

	AActor::~AActor() = default;

	void AActor::BeginPlay() const
	{
		for (auto& c : comps_)
			c->BeginPlay();
	}

	Vector3f AActor::GetForward() const noexcept
	{
		return math::R2V(rot_);
	}

	void AActor::Update(const float deltaSeconds)
	{
		UpdateLifespan(deltaSeconds);
		UpdateComponents(deltaSeconds);
		UpdateActor(deltaSeconds);
	}

	void AActor::UpdateComponents(const float deltaSeconds)
	{
		for (const auto& comp : comps_)
		{
			comp->Update(deltaSeconds);
		}
	}

	void AActor::UpdateLifespan(const float deltaSeconds)
	{
		if (lifespan_ > 0 && (lifespan_ -= deltaSeconds) <= 0)
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

	void AActor::SetPos(const Vector3f& new_pos, bool recompute_world_transform) noexcept
	{
		pos_ = new_pos;
		if (recompute_world_transform) RecomputeWorldTransform();
	}

	void AActor::SetRot(Degrees new_rot, bool recompute_world_transform) noexcept
	{
		rot_ = new_rot;
		if (recompute_world_transform) RecomputeWorldTransform();
	}
	
	void AActor::SetScale(const Vector3f& scale, bool recompute_world_transform) noexcept
	{
		scale_ = scale;
		if (recompute_world_transform) RecomputeWorldTransform();
	}

	void AActor::RecomputeWorldTransform() noexcept
	{
	}
}
