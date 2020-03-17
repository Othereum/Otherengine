#pragma once

namespace Game
{
	class AActor;
	class CEngine;
	
	class CActorComponent
	{
	public:
		explicit CActorComponent(AActor& owner, int updateOrder = 100);
		virtual ~CActorComponent() = default;

		virtual void BeginPlay() {}
		virtual void Update(float deltaSeconds) {}
		[[nodiscard]] int GetUpdateOrder() const noexcept { return updateOrder_; }
		
		[[nodiscard]] AActor& GetOwner() const noexcept { return owner_; }
		[[nodiscard]] CEngine& GetEngine() const noexcept;

	private:
		AActor& owner_;
		int updateOrder_;
	};
}