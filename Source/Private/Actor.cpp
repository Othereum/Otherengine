#include "Actor.h"
#include "Components/ActorComponent.h"
#include "MathUtil.h"
#include "World.h"

namespace oeng
{
	AActor::AActor(CWorld& world):
		world_{world}
	{
	}

	AActor::~AActor() = default;

	void AActor::BeginPlay() const
	{
		for (const auto& c : comps_)
			c->BeginPlay();
	}

	Vec2 AActor::GetForward() const noexcept
	{
		return math::R2V(GetRot());
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

	void AActor::SetTransform(const Transform& new_transform, bool recompute_matrix) noexcept
	{
		world_transform_ = new_transform;
		if (recompute_matrix) RecomputeMatrix();
	}

	void AActor::SetPos(const Vec2& new_pos, bool recompute_matrix) noexcept
	{
		world_transform_.pos = new_pos;
		if (recompute_matrix) RecomputeMatrix();
	}

	void AActor::SetRot(Degrees new_rot, bool recompute_matrix) noexcept
	{
		world_transform_.rot = new_rot;
		if (recompute_matrix) RecomputeMatrix();
	}
	
	void AActor::SetScale(const Vec2& scale, bool recompute_matrix) noexcept
	{
		world_transform_.scale = scale;
		if (recompute_matrix) RecomputeMatrix();
	}

	void AActor::RecomputeMatrix() noexcept
	{
		transform_matrix_ = world_transform_.ToMatrix();
	}
}
