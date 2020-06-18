#pragma once
#include "MathFwd.hpp"
#include "Templates/Memory.hpp"

namespace oeng
{
	class AActor;
	class Engine;
	class World;
	class Renderer;
	
	class ActorComponent : public EnableSharedFromThis<ActorComponent>
	{
	public:
		OE_DELETE_CPMV(ActorComponent);

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
		[[nodiscard]] Engine& GetEngine() const noexcept;
		[[nodiscard]] World& GetWorld() const noexcept;
		[[nodiscard]] Renderer& GetRenderer() const noexcept;

	private:
		virtual void OnBeginPlay() {}
		virtual void OnUpdate(Float delta_seconds) {}
		virtual void OnActivated() {}
		virtual void OnDeactivated() {}
		
		AActor& owner_;
		int update_order_;
		bool auto_activate_ : 1 = true;
		bool is_active_ : 1 = false;
		bool begun_play_ : 1 = false;
	};
}