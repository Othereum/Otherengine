#pragma once
#include "Components/ActorComponent.hpp"

namespace oeng
{
	class OEAPI MovementComponent : public ActorComponent
	{
	public:
		explicit MovementComponent(AActor& owner, int update_order = 1000);
		void AddInput(const Vec3& input);

	private:
		void OnUpdate(Float delta_seconds) override;
		
		Float max_speed_ = 300;
		Vec3 input_;
	};
}