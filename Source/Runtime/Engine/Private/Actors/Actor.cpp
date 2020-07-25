#include "Actors/Actor.hpp"
#include "Stat.hpp"
#include "World.hpp"
#include "Components/SceneComponent.hpp"

namespace oeng::engine
{
	AActor::AActor(World& world)
		:world_{world}
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

		OnBeginPlay();
	}

	void AActor::Update(const Float delta_seconds)
	{
		SCOPE_STACK_COUNTER(ActorUpdate);
		UpdateComponents(delta_seconds);
		OnUpdate(delta_seconds);
	}

	void AActor::UpdateComponents(const Float delta_seconds)
	{
		for (const auto& comp : comps_) comp->Update(delta_seconds);
	}

	TimerManager& AActor::GetTimerManager() const noexcept
	{
		return world_.GetTimerManager();
	}

	void AActor::RegisterComponent(SharedRef<ActorComponent>&& comp)
	{
		auto cmp = [](auto&& a, auto&& b) {
			return a->GetUpdateOrder() <= b->GetUpdateOrder();
		};
		const auto pos = std::lower_bound(comps_.begin(), comps_.end(), comp, cmp);
		comps_.insert(pos, std::move(comp));
	}

	void AActor::Destroy()
	{
		pending_kill_ = true;
	}

	void AActor::SetRootComponent(SceneComponent* new_root) noexcept
	{
		assert(!new_root || &new_root->GetOwner() == this);
		root_ = new_root;
	}

	void AActor::SetLifespan(Float in_seconds)
	{
		init_lifespan_ = in_seconds;
		if (!begun_play_) return;

		auto& timer = GetWorld().GetTimerManager();
		if (timer.IsTimerExists(lifespan_timer_))
		{
			if (init_lifespan_ > 0) timer.UpdateTimer(lifespan_timer_, init_lifespan_);
			else timer.RemoveTimer(lifespan_timer_);
		}
		else if (init_lifespan_ > 0)
		{
			lifespan_timer_ = timer.SetTimer(init_lifespan_, [self = weak_from_this()]
			{
				if (auto ptr = self.lock()) ptr->Destroy();
			});
		}
	}

	Float AActor::GetLifespan() const noexcept
	{
		auto& timer = GetWorld().GetTimerManager();
		return timer.IsTimerExists(lifespan_timer_) ? timer.TimeLeft(lifespan_timer_) : 0;
	}

	void AActor::SetTrsf(const Transform& trsf) const noexcept { if (root_) root_->SetRelTrsf(trsf); }
	void AActor::SetPos(const Vec3& pos) const noexcept { if (root_) root_->SetRelPos(pos); }
	void AActor::SetRot(const Quat& rot) const noexcept { if (root_) root_->SetRelRot(rot); }
	void AActor::SetScale(const Vec3& scale) const noexcept { if (root_) root_->SetRelScale(scale); }

	const Transform& AActor::GetTrsf() const noexcept { return root_ ? root_->GetRelTrsf() : Transform::identity; }
	const Vec3& AActor::GetPos() const noexcept { return root_ ? root_->GetRelPos() : Vec3::zero; }
	const Quat& AActor::GetRot() const noexcept { return root_ ? root_->GetRelRot() : Quat::identity; }
	const Vec3& AActor::GetScale() const noexcept { return root_ ? root_->GetRelScale() : Vec3::one; }

	UVec3 AActor::GetForward() const noexcept { return root_ ? root_->GetForward() : UVec3::forward; }
	UVec3 AActor::GetBackward() const noexcept { return root_ ? root_->GetBackward() : UVec3::backward; }
	UVec3 AActor::GetRight() const noexcept { return root_ ? root_->GetRight() : UVec3::right; }
	UVec3 AActor::GetLeft() const noexcept { return root_ ? root_->GetLeft() : UVec3::left; }
	UVec3 AActor::GetUp() const noexcept { return root_ ? root_->GetUp() : UVec3::up; }
	UVec3 AActor::GetDown() const noexcept { return root_ ? root_->GetDown() : UVec3::down; }
}
