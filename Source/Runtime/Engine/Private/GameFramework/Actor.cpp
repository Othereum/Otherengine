#include "GameFramework/Actor.hpp"
#include "Stat.hpp"
#include "World.hpp"
#include "Components/SceneComponent.hpp"

namespace logcat
{
const LogCategory kActor{u8"Actor"sv};
}

namespace oeng
{
inline namespace engine
{
AActor::~AActor()
{
    GetWorld().GetTimerManager().RemoveTimer(lifespan_timer_);
}

void AActor::BeginPlay()
{
    begun_play_ = true;

    for (const auto& c : comps_)
        c->BeginPlay();

    SetLifespan(init_lifespan_);
    OnBeginPlay();
}

void AActor::Update(Float delta_seconds)
{
    if (update_enabled_)
    {
        SCOPE_STACK_COUNTER(ActorUpdate);
        UpdateComponents(delta_seconds);
        OnUpdate(delta_seconds);
    }
}

void AActor::UpdateComponents(Float delta_seconds)
{
    for (const auto& comp : comps_)
        comp->Update(delta_seconds);
}

void AActor::RegisterComponent(std::shared_ptr<ActorComponent>&& comp)
{
    auto cmp = [](const std::shared_ptr<ActorComponent>& a, const std::shared_ptr<ActorComponent>& b)
    {
        return a->update_order_ < b->update_order_;
    };
    const auto pos = std::upper_bound(comps_.begin(), comps_.end(), comp, cmp);
    comps_.insert(pos, std::move(comp));

    comp->owner_ = this;
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

    if (!begun_play_)
        return;

    auto& timer = GetWorld().GetTimerManager();
    if (timer.IsTimerExists(lifespan_timer_))
    {
        if (init_lifespan_ > 0)
            timer.UpdateTimer(lifespan_timer_, init_lifespan_);
        else
            timer.RemoveTimer(lifespan_timer_);
    }
    else if (init_lifespan_ > 0)
    {
        lifespan_timer_ = timer.SetTimer(init_lifespan_, [self = weak_from_this()]
        {
            if (auto ptr = self.lock())
                ptr->Destroy();
        });
    }
}

Float AActor::GetLifespan() const noexcept
{
    auto& timer = GetWorld().GetTimerManager();
    return timer.IsTimerExists(lifespan_timer_) ? timer.TimeLeft(lifespan_timer_) : 0;
}

void AActor::SetCanEverUpdate(bool can_ever_update) noexcept
{
    if (ENSURE(!begun_play_))
    {
        can_ever_update_ = can_ever_update;
    }
}

void AActor::SetUpdateEnabled(bool update_enabled) noexcept
{
    EXPECT(!begun_play_ || can_ever_update_ || !update_enabled);
    update_enabled_ = update_enabled;
}

void AActor::SetTrsf(const Transform& trsf) const noexcept
{
    if (root_)
        root_->SetRelTrsf(trsf);
}

void AActor::SetPos(const Vec3& pos) const noexcept
{
    if (root_)
        root_->SetRelPos(pos);
}

void AActor::SetRot(const Quat& rot) const noexcept
{
    if (root_)
        root_->SetRelRot(rot);
}

void AActor::SetScale(const Vec3& scale) const noexcept
{
    if (root_)
        root_->SetRelScale(scale);
}

const Transform& AActor::GetTrsf() const noexcept
{
    return root_ ? root_->GetRelTrsf() : Transform::identity;
}

const Vec3& AActor::GetPos() const noexcept
{
    return root_ ? root_->GetRelPos() : Vec3::zero;
}

const Quat& AActor::GetRot() const noexcept
{
    return root_ ? root_->GetRelRot() : Quat::identity;
}

const Vec3& AActor::GetScale() const noexcept
{
    return root_ ? root_->GetRelScale() : Vec3::one;
}

UVec3 AActor::GetForward() const noexcept
{
    return root_ ? root_->GetForward() : UVec3::forward;
}

UVec3 AActor::GetRight() const noexcept
{
    return root_ ? root_->GetRight() : UVec3::right;
}

UVec3 AActor::GetUp() const noexcept
{
    return root_ ? root_->GetUp() : UVec3::up;
}
}
}
