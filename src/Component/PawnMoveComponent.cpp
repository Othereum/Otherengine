#include "Component/PawnMoveComponent.h"

namespace Game
{
	CPawnMoveComp::CPawnMoveComp(AActor& owner, int updateOrder)
		:CMoveComponent{owner, updateOrder}
	{
	}

	void CPawnMoveComp::Update(float deltaSeconds)
	{
		const auto lensqr = input_.LenSqr();
		if (lensqr > 1) input_ /= sqrtf(lensqr);

		rotInput_ = Math::Clamp(rotInput_, -1, 1);

		SetVelocity(input_ * maxSpeed_);
		SetAngularVelocity(rotInput_ * maxRotSpeed_);

		input_ = {};
		rotInput_ = {};

		CMoveComponent::Update(deltaSeconds);
	}
}
