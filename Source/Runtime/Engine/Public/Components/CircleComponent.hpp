#pragma once
#include <functional>
#include "ActorComponent.hpp"

namespace oeng::engine
{
	class CircleComponent : public ActorComponent
	{
	public:
		explicit CircleComponent(AActor& owner, int update_order = 100);
		~CircleComponent();

		void TestOverlap(CircleComponent& other);
		void BindOnOverlap(std::function<void(CircleComponent&)>&& on_overlap) noexcept;

		void SetRadius(Float r) noexcept { radius_ = r; }
		[[nodiscard]] Float GetRadius() const noexcept { return radius_; }

	private:
		void OnBeginPlay() override;
		
		std::function<void(CircleComponent&)> on_overlap_;
		Float radius_ = 32;
	};
}
