#pragma once
#include "ActorComponent.h"
#include "Rotation.h"
#include "Vector.hpp"

namespace oeng
{
	class CMoveComponent : public CActorComponent
	{
	public:
		explicit CMoveComponent(AActor& owner, int update_order = 10);

		void Update(float delta_seconds) override;

		void SetMass(float mass) noexcept { mass_ = mass; }
		void AddForce(Vec2 force) noexcept { force_ += force; }
		void SetVelocity(Vec2 newvel) noexcept { vel_ = newvel; }
		void SetAngularVelocity(Degrees newvel) noexcept { rvel_ = newvel; }

		[[nodiscard]] float GetMass() const noexcept { return mass_; }
		[[nodiscard]] Vec2 GetForce() const noexcept { return force_; }
		[[nodiscard]] Vec2 GetVelocity() const noexcept { return vel_; }
		[[nodiscard]] Degrees GetAngularVelocity() const noexcept { return rvel_; }

	private:
		float mass_ = 1;
		Vec2 force_;
		Vec2 vel_;
		Degrees rvel_;
	};
}
