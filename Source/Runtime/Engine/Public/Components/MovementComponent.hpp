#pragma once
#include "Components/ActorComponent.hpp"
#include "Math.hpp"

ENGINE_BEGIN

class SceneComponent;

class ENGINE_API MovementComponent : public ActorComponent
{
public:
	explicit MovementComponent(AActor& owner, int update_order = 1000);
	
	void AddMovInput(const Vec3& input) noexcept { mov_input_ += input; }
	void AddRotInput(const Quat& input) noexcept { rot_input_ = input * rot_input_; }

	void SetMaxSpeed(Float speed) noexcept { max_speed_ = speed; }
	[[nodiscard]] Float GetMaxSpeed() const noexcept { return max_speed_; }

protected:
	void OnUpdate(Float delta_seconds) override;

private:
	bool Move(Transform& trsf, Float delta_seconds) noexcept;
	bool Rotate(Transform& trsf) noexcept;
	
	Float max_speed_ = 300;
	Vec3 mov_input_;
	Quat rot_input_;
};

ENGINE_END
