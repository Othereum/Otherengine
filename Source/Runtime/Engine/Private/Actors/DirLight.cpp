#include "Actors/DirLight.hpp"
#include "Components/DirLightComponent.hpp"

namespace oeng
{
	ADirLight::ADirLight(World& world)
		:AActor{world}, light_{AddComponent<DirLightComponent>()}
	{
		SetRootComponent(&light_);
	}

	void ADirLight::Activate() const noexcept
	{
		light_.Activate();
	}

	void ADirLight::SetColor(const Vec3& color) const noexcept
	{
		light_.SetColor(color);
	}

	const Vec3& ADirLight::GetColor() const noexcept
	{
		return light_.GetColor();
	}
}
