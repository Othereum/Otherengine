#pragma once
#include "ActorComponent.h"
#include "Rotation.h"
#include "Vector.h"

namespace game
{
	class CMoveComponent : public CActorComponent
	{
	public:
		explicit CMoveComponent(AActor& owner, int update_order = 10);

		void Update(float delta_seconds) override;

		void SetMass(float mass) noexcept { mass_ = mass; }
		void AddForce(FVector2 force) noexcept { force_ += force; }
		void SetVelocity(FVector2 newvel) noexcept { vel_ = newvel; }
		void SetAngularVelocity(Degrees newvel) noexcept { rvel_ = newvel; }

		[[nodiscard]] float GetMass() const noexcept { return mass_; }
		[[nodiscard]] FVector2 GetForce() const noexcept { return force_; }
		[[nodiscard]] FVector2 GetVelocity() const noexcept { return vel_; }
		[[nodiscard]] Degrees GetAngularVelocity() const noexcept { return rvel_; }

	private:
		float mass_ = 1;
		FVector2 force_;
		FVector2 vel_;
		Degrees rvel_;
	};
}
