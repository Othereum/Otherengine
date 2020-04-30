#include "Actor.h"
#include "Components/ActorComponent.h"
#include "World.h"

namespace oeng
{
	AActor::AActor(CWorld& world):
		world_{world}
	{
	}

	AActor::~AActor()
	{
		GetWorld().GetTimerManager().RemoveTimer(lifespan_timer_);
	}

	void AActor::BeginPlay()
	{
		for (const auto& c : comps_)
			c->BeginPlay();

		begun_play_ = true;
		SetLifespan(init_lifespan_);
	}

	void AActor::Update(const float delta_seconds)
	{
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

	CEngine& AActor::GetEngine() const noexcept
	{
		return world_.GetEngine();
	}

	TimerManager& AActor::GetTimerManager() const noexcept
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
		pending_kill_ = true;
	}

	void AActor::SetEnabled(bool enable)
	{
		for (auto& c : comps_)
		{
			c->SetEnabled(enable);
		}
	}

	void AActor::SetLifespan(float in_seconds)
	{
		init_lifespan_ = in_seconds;
		if (!begun_play_) return;

		auto& timer = GetWorld().GetTimerManager();
		if (timer.IsTimerExists(lifespan_timer_))
		{
			if (init_lifespan_ > 0)
			{
				timer.UpdateTimer(lifespan_timer_, init_lifespan_);
			}
			else
			{
				timer.RemoveTimer(lifespan_timer_);
			}
		}
		else if (init_lifespan_ > 0)
		{
			lifespan_timer_ = timer.SetTimer(init_lifespan_, [self = weak_from_this()]
			{
				if (auto ptr = self.lock())
				{
					ptr->Destroy();
				}
			});
		}
	}

	float AActor::GetLifespan() const noexcept
	{
		auto& timer = GetWorld().GetTimerManager();
		return timer.IsTimerExists(lifespan_timer_) ? timer.TimeLeft(lifespan_timer_) : 0;
	}

	void AActor::SetTransform(const Transform& new_transform, bool recompute_matrix) noexcept
	{
		world_transform_ = new_transform;
		if (recompute_matrix) RecomputeMatrix();
	}

	void AActor::SetPos(const Vec3& new_pos, bool recompute_matrix) noexcept
	{
		world_transform_.pos = new_pos;
		if (recompute_matrix) RecomputeMatrix();
	}

	void AActor::SetRot(const Quat& new_rot, bool recompute_matrix) noexcept
	{
		world_transform_.rot = new_rot;
		if (recompute_matrix) RecomputeMatrix();
	}
	
	void AActor::SetScale(const Vec3& scale, bool recompute_matrix) noexcept
	{
		world_transform_.scale = scale;
		if (recompute_matrix) RecomputeMatrix();
	}

	void AActor::RecomputeMatrix() noexcept
	{
		transform_matrix_ = world_transform_.ToMatrix();
	}
}
