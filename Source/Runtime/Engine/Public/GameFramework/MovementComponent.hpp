#pragma once
#include "Components/ActorComponent.hpp"
#include "Math.hpp"

namespace oeng
{
	class OEAPI MovementComponent : public ActorComponent
	{
	public:
		explicit MovementComponent(AActor& owner, int update_order = 1000);
		void AddInput(const Vec3& input) { input_ += input; }

		void SetMaxSpeed(Float speed) noexcept { max_speed_ = speed; }
		[[nodiscard]] Float GetMaxSpeed() const noexcept { return max_speed_; }

	private:
		void OnUpdate(Float delta_seconds) override;
		
		Float max_speed_ = 300;
		Vec3 input_;
	};
}