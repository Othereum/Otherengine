#include "Engine/DirLight.hpp"
#include "Components/DirLightComponent.hpp"

namespace oeng
{
inline namespace engine
{
ADirLight::ADirLight()
    : light_{AddComponent<DirLightComponent>()}
{
    SetRootComponent(&light_);
}

void ADirLight::SetColor(const Vec3& color) const noexcept
{
    light_.color = color;
}

const Vec3& ADirLight::GetColor() const noexcept
{
    return light_.color;
}
}
