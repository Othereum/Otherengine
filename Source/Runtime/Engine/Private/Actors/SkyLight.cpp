#include "Actors/SkyLight.hpp"
#include "Components/SkyLightComponent.hpp"

namespace oeng::engine
{
	ASkyLight::ASkyLight(World& world)
		:AActor{world}, light_{AddComponent<SkyLightComponent>()}
	{
		SetRootComponent(&light_);
	}

	void ASkyLight::Activate() const noexcept
	{
		light_.Activate();
	}

	void ASkyLight::SetColor(const Vec3& color) const noexcept
	{
		light_.SetColor(color);
	}

	const Vec3& ASkyLight::GetColor() const noexcept
	{
		return light_.GetColor();
	}
}
