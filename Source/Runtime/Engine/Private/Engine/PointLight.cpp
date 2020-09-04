#include "Engine/PointLight.hpp"
#include "Components/PointLightComponent.hpp"

namespace oeng
{
inline namespace engine
{
APointLight::APointLight()
    : light_{AddComponent<PointLightComponent>()}
{
    SetRootComponent(&light_);
}

void APointLight::SetRadius(Float radius) const noexcept
{
    light_.radius = radius;
}

void APointLight::SetColor(const Vec3& color) const noexcept
{
    light_.color = color;
}

Float APointLight::GetRadius() const noexcept
{
    return light_.radius;
}

const Vec3& APointLight::GetColor() const noexcept
{
    return light_.color;
}
}
}
