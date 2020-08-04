#pragma once
#include "Actor.hpp"

ENGINE_BEGIN

class PointLightComponent;

class ENGINE_API APointLight : public AActor
{
public:
	explicit APointLight(World& world);

	void SetRadius(float radius) const noexcept;
	void SetColor(const Vec3& color) const noexcept;
	[[nodiscard]] float GetRadius() const noexcept;
	[[nodiscard]] const Vec3& GetColor() const noexcept;

private:
	PointLightComponent& light_;
};

ENGINE_END
