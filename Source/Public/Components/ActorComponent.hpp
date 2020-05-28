#pragma once
#include "Templates/SharedPtr.hpp"

namespace oeng
{
	class AActor;
	class Engine;
	class CWorld;
	
	class ActorComponent : public EnableSharedFromThis<ActorComponent>
	{
	public:
		explicit ActorComponent(AActor& owner, int update_order = 100);
		virtual ~ActorComponent() = default;

		virtual void BeginPlay() {}
		virtual void Update(float delta_seconds) {}
		
		void SetEnabled(bool enable) noexcept { is_enabled_ = enable; }
		[[nodiscard]] bool IsEnabled() const noexcept { return is_enabled_; }
		
		[[nodiscard]] int GetUpdateOrder() const noexcept { return update_order_; }
		[[nodiscard]] AActor& GetOwner() const noexcept { return owner_; }
		[[nodiscard]] Engine& GetEngine() const noexcept;
		[[nodiscard]] CWorld& GetWorld() const noexcept;

	private:
		AActor& owner_;
		int update_order_;
		bool is_enabled_ = true;
	};
}