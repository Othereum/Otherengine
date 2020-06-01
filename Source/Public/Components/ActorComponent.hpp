#pragma once
#include "Templates/Memory.hpp"

namespace oeng
{
	class AActor;
	class Engine;
	class World;
	
	class ActorComponent : public EnableSharedFromThis<ActorComponent>
	{
	public:
		explicit ActorComponent(AActor& owner, int update_order = 100);
		virtual ~ActorComponent() = default;

		ActorComponent(const ActorComponent&) = delete;
		ActorComponent(ActorComponent&&) = delete;
		ActorComponent& operator=(const ActorComponent&) = delete;
		ActorComponent& operator=(ActorComponent&&) = delete;

		void BeginPlay();
		void Update(float delta_seconds);
		
		void SetEnabled(bool enable) noexcept { is_enabled_ = enable; }
		[[nodiscard]] bool IsEnabled() const noexcept { return is_enabled_; }
		
		[[nodiscard]] int GetUpdateOrder() const noexcept { return update_order_; }
		[[nodiscard]] AActor& GetOwner() const noexcept { return owner_; }
		[[nodiscard]] Engine& GetEngine() const noexcept;
		[[nodiscard]] World& GetWorld() const noexcept;

	private:
		virtual void OnBeginPlay() {}
		virtual void OnUpdate(float delta_seconds) {}
		
		AActor& owner_;
		int update_order_;
		bool is_enabled_ = true;
	};
}