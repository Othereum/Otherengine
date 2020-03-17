#pragma once
#include <functional>
#include "ActorComponent.h"

namespace Game
{
	class CCircleComponent : public CActorComponent
	{
	public:
		explicit CCircleComponent(AActor& owner, int updateOrder = 100);
		~CCircleComponent();

		void BeginPlay() override;
		
		void TestOverlap(CCircleComponent& other);
		void BindOnOverlap(std::function<void(CCircleComponent&)>&& onOverlap) noexcept;

		void SetRadius(float r) noexcept { radius_ = r; }
		[[nodiscard]] float GetRadius() const noexcept { return radius_; }

	private:
		std::function<void(CCircleComponent&)> onOverlap_;
		float radius_ = 32;
	};
}
