#include "Components/SkyLightComponent.hpp"
#include "Engine/Engine.hpp"
#include "IRenderer.hpp"

namespace oeng
{
inline namespace engine
{

void SkyLightComponent::OnBeginPlay()
{
    GetRenderer().AddSkyLight(*this);
}

void SkyLightComponent::OnEndPlay()
{
    GetRenderer().RemoveSkyLight(*this);
}

IRenderer& SkyLightComponent::GetRenderer() const noexcept
{
    return GetEngine().GetRenderer();
}

} // namespace engine
} // namespace oeng
