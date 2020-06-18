#pragma once
#include "MathFwd.hpp"
#include "Templates/Memory.hpp"

namespace oeng
{
	class AActor;
	class Engine;
	class World;
	
	class ActorComponent : public EnableSharedFromThis<ActorComponent>
	{
	public:
		OE_DELETE_CPMV(ActorComponent);

		explicit ActorComponent(AActor& owner, int update_order = 100);
		virtual ~ActorComponent() = default;

		void BeginPlay();
		void Update(Float delta_seconds);
		
		void SetEnabled(bool enable) noexcept { is_enabled_ = enable; }
		[[nodiscard]] bool IsEnabled() const noexcept { return is_enabled_; }
		[[nodiscard]] bool HasBegunPlay() const noexcept { return begun_play_; }
		
		[[nodiscard]] int GetUpdateOrder() const noexcept { return update_order_; }
		[[nodiscard]] AActor& GetOwner() const noexcept { return owner_; }
		[[nodiscard]] Engine& GetEngine() const noexcept;
		[[nodiscard]] World& GetWorld() const noexcept;

	private:
		virtual void OnBeginPlay() {}
		virtual void OnUpdate(Float delta_seconds) {}
		
		AActor& owner_;
		int update_order_;
		bool is_enabled_ : 1 = true;
		bool begun_play_ : 1 = false;
	};
}