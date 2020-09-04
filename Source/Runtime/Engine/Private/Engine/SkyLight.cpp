#include "Engine/SkyLight.hpp"
#include "Components/SkyLightComponent.hpp"

namespace oeng
{
inline namespace engine
{
ASkyLight::ASkyLight()
    : light_{AddComponent<SkyLightComponent>()}
{
    SetRootComponent(&light_);
}

void ASkyLight::SetColor(const Vec3& color) const noexcept
{
    light_.color = color;
}

const Vec3& ASkyLight::GetColor() const noexcept
{
    return light_.color;
}
}
}
