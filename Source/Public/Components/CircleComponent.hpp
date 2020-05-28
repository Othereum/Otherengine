#pragma once
#include <functional>
#include "ActorComponent.hpp"

namespace oeng
{
	class CircleComponent : public ActorComponent
	{
	public:
		explicit CircleComponent(AActor& owner, int update_order = 100);
		~CircleComponent();

		void TestOverlap(CircleComponent& other);
		void BindOnOverlap(std::function<void(CircleComponent&)>&& on_overlap) noexcept;

		void SetRadius(float r) noexcept { radius_ = r; }
		[[nodiscard]] float GetRadius() const noexcept { return radius_; }

	private:
		void OnBeginPlay() override;
		
		std::function<void(CircleComponent&)> on_overlap_;
		float radius_ = 32;
	};
}
