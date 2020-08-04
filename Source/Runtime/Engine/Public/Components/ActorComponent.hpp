#pragma once
#include "MathFwd.hpp"
#include "Templates/Pointer.hpp"

RENDERER_BEGIN
	class Renderer;
RENDERER_END


ENGINE_BEGIN

class AActor;
class World;

class ENGINE_API ActorComponent : public EnableSharedFromThis<ActorComponent>
{
public:
	DELETE_CPMV(ActorComponent);

	explicit ActorComponent(AActor& owner, int update_order = 100);
	virtual ~ActorComponent() = default;

	void BeginPlay();
	void Update(Float delta_seconds);

	void Activate() { if (!is_active_) is_active_ = true, OnActivated(); }
	void Deactivate() { if (is_active_) is_active_ = false, OnDeactivated(); }
	void SetAutoActivate(bool auto_activate) noexcept { auto_activate_ = auto_activate; }
	[[nodiscard]] bool IsActive() const noexcept { return is_active_; }
	[[nodiscard]] bool HasBegunPlay() const noexcept { return begun_play_; }
	[[nodiscard]] bool IsAutoActivate() const noexcept { return auto_activate_; }
	
	[[nodiscard]] int GetUpdateOrder() const noexcept { return update_order_; }
	[[nodiscard]] AActor& GetOwner() const noexcept { return owner_; }
	[[nodiscard]] World& GetWorld() const noexcept;
	[[nodiscard]] static Renderer& GetRenderer() noexcept;

protected:
	virtual void OnBeginPlay() {}
	virtual void OnUpdate([[maybe_unused]] Float delta_seconds) {}
	virtual void OnActivated() {}
	virtual void OnDeactivated() {}

private:
	AActor& owner_;
	int update_order_;
	bool auto_activate_ : 1 = true;
	bool is_active_ : 1 = false;
	bool begun_play_ : 1 = false;
};

ENGINE_END
