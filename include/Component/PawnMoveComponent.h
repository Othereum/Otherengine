#pragma once
#include "MoveComponent.h"

namespace Game
{
	class CPawnMoveComp : public CMoveComponent
	{
	public:
		explicit CPawnMoveComp(AActor& owner, int updateOrder = 10);

		void Update(float deltaSeconds) override;

		void AddInput(TFVector2 v) noexcept { input_ += v; }
		void AddRotationInput(float r) noexcept { rotInput_ += r; }

		void SetMaxSpeed(float s) noexcept { maxSpeed_ = s; }
		void SetMaxRotationSpeed(TDegrees s) noexcept { maxRotSpeed_ = s; }

	private:
		TFVector2 input_;
		float rotInput_ = 0;
		
		float maxSpeed_ = 300;
		TDegrees maxRotSpeed_ = 360_deg;
	};
}
