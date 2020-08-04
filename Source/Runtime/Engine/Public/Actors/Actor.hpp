#pragma once
#include "Math.hpp"
#include "Name.hpp"
#include "TimerManager.hpp"
#include "Templates/DyArr.hpp"
#include "Templates/HashSet.hpp"
#include "Templates/Pointer.hpp"

ENGINE_BEGIN

class Engine;
class World;
class ActorComponent;
class SceneComponent;
class TimerManager;

class ENGINE_API AActor : public EnableSharedFromThis<AActor>
{
public:
	DELETE_CPMV(AActor);
	
	explicit AActor(World& world);
	virtual ~AActor();

	void BeginPlay();
	void Update(Float delta_seconds);
	void Destroy();

	template <std::derived_from<ActorComponent> T, class... Args>
	SharedRef<T> AddComponent(Args&&... args)
	{
		static_assert(std::is_constructible_v<T, AActor&, Args...>);
		auto ptr = MakeShared<T>(*this, std::forward<Args>(args)...);
		RegisterComponent(ptr);
		return ptr;
	}

	void AddTag(Name tag) { tags_.insert(tag); }
	void RemoveTag(Name tag) { tags_.erase(tag); }
	[[nodiscard]] bool HasTag(Name tag) const noexcept { return tags_.contains(tag); }

	void SetUpdateEnabled(bool enabled) noexcept { update_enabled_ = enabled; }

	/**
	 * Set root component of this actor. Root component represents this actor's transform.
	 * @param new_root New root component. It can be nullptr or MUST be owned by this actor. 
	 */
	void SetRootComponent(SceneComponent* new_root) noexcept;
	[[nodiscard]] SceneComponent* GetRootComponent() const noexcept { return root_; }

	[[nodiscard]] bool IsPendingKill() const noexcept { return pending_kill_; }

	/**
	 * Set actor's lifespan. Default is 0 (infinite). Timer is updated when called.
	 * @param in_seconds New lifespan in seconds. <=0 means infinite.
	 */
	void SetLifespan(Float in_seconds);
	[[nodiscard]] Float GetLifespan() const noexcept;
	[[nodiscard]] Float GetInitialLifespan() const noexcept { return init_lifespan_; }

	void SetTrsf(const Transform& trsf) const noexcept;
	[[nodiscard]] const Transform& GetTrsf() const noexcept;
	
	void SetPos(const Vec3& pos) const noexcept;
	[[nodiscard]] const Vec3& GetPos() const noexcept;

	void SetRot(const Quat& rot) const noexcept;
	[[nodiscard]] const Quat& GetRot() const noexcept;

	void SetScale(const Vec3& scale) const noexcept;
	[[nodiscard]] const Vec3& GetScale() const noexcept;

	[[nodiscard]] UVec3 GetForward() const noexcept;
	[[nodiscard]] UVec3 GetBackward() const noexcept;
	[[nodiscard]] UVec3 GetRight() const noexcept;
	[[nodiscard]] UVec3 GetLeft() const noexcept;
	[[nodiscard]] UVec3 GetUp() const noexcept;
	[[nodiscard]] UVec3 GetDown() const noexcept;

	[[nodiscard]] World& GetWorld() const noexcept { return world_; }
	[[nodiscard]] TimerManager& GetTimerManager() const noexcept;

protected:
	virtual void OnUpdate([[maybe_unused]] Float delta_seconds) {}
	virtual void OnBeginPlay() {}

private:
	void RegisterComponent(SharedRef<ActorComponent> comp);
	void UpdateComponents(Float delta_seconds);

	bool pending_kill_ : 1 = false;
	bool begun_play_ : 1 = false;
	bool update_enabled_ : 1 = true;
	
	Float init_lifespan_ = 0;
	TimerHandle lifespan_timer_;
	
	World& world_;
	DyArr<SharedRef<ActorComponent>> comps_;
	SceneComponent* root_ = nullptr;

	HashSet<Name> tags_;
};

ENGINE_END
