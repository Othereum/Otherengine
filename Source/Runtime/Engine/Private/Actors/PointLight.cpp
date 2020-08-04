#include "Actors/PointLight.hpp"
#include "Components/PointLightComponent.hpp"

namespace oeng::engine
{
	APointLight::APointLight(World& world)
		:AActor{world}, light_{*AddComponent<PointLightComponent>()}
	{
		SetRootComponent(&light_);
	}

	void APointLight::SetRadius(float radius) const noexcept
	{
		light_.SetRadius(radius);
	}

	void APointLight::SetColor(const Vec3& color) const noexcept
	{
		light_.SetColor(color);
	}

	float APointLight::GetRadius() const noexcept
	{
		return light_.GetRadius();
	}

	const Vec3& APointLight::GetColor() const noexcept
	{
		return light_.GetColor();
	}
}
