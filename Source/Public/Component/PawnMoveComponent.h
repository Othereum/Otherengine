#pragma once
#include "MoveComponent.h"

namespace oeng
{
	class CPawnMoveComp : public CMoveComponent
	{
	public:
		explicit CPawnMoveComp(AActor& owner, int updateOrder = 10);

		void Update(float deltaSeconds) override;

		void AddInput(Vec2 v) noexcept { input_ += v; }
		void AddRotationInput(float r) noexcept { rotInput_ += r; }

		void SetMaxSpeed(float s) noexcept { maxSpeed_ = s; }
		void SetMaxRotationSpeed(Degrees s) noexcept { maxRotSpeed_ = s; }

	private:
		Vec2 input_;
		float rotInput_ = 0;
		
		float maxSpeed_ = 300;
		Degrees maxRotSpeed_ = 360_deg;
	};
}
