#pragma once

namespace oeng
{
	class AActor;
	class CEngine;
	class CWorld;
	
	class CActorComponent
	{
	public:
		explicit CActorComponent(AActor& owner, int updateOrder = 100);
		virtual ~CActorComponent() = default;

		virtual void BeginPlay() {}
		virtual void Update(float deltaSeconds) {}
		
		void SetEnabled(bool enable) noexcept { is_enabled_ = enable; }
		[[nodiscard]] bool IsEnabled() const noexcept { return is_enabled_; }
		
		[[nodiscard]] int GetUpdateOrder() const noexcept { return updateOrder_; }
		[[nodiscard]] AActor& GetOwner() const noexcept { return owner_; }
		[[nodiscard]] CEngine& GetEngine() const noexcept;
		[[nodiscard]] CWorld& GetWorld() const noexcept;

	private:
		AActor& owner_;
		int updateOrder_;
		bool is_enabled_ = true;
	};
}