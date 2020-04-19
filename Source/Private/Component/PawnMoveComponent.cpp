#include "Components/PawnMoveComponent.h"

namespace oeng
{
	CPawnMoveComp::CPawnMoveComp(AActor& owner, int updateOrder)
		:CMoveComponent{owner, updateOrder}
	{
	}

	void CPawnMoveComp::Update(float deltaSeconds)
	{
		const auto lensqr = input_.LenSqr();
		if (lensqr > 1) input_ /= sqrtf(lensqr);

		rotInput_ = math::Clamp(rotInput_, -1, 1);

		SetVelocity(input_ * maxSpeed_);
		SetAngularVelocity(rotInput_ * maxRotSpeed_);

		input_ = {};
		rotInput_ = {};

		CMoveComponent::Update(deltaSeconds);
	}
}
