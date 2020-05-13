#pragma once

namespace oeng
{
	class AActor;
	class Engine;
	class CWorld;
	
	class CActorComponent
	{
	public:
		explicit CActorComponent(AActor& owner, int update_order = 100);
		virtual ~CActorComponent() = default;

		virtual void BeginPlay() {}
		virtual void Update(float deltaSeconds) {}
		
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