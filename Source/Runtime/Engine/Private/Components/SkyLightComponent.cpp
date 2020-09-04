#include "Components/SkyLightComponent.hpp"
#include "IRenderer.hpp"

namespace oeng
{
inline namespace engine
{
void SkyLightComponent::OnBeginPlay()
{
    IRenderer::Get().AddSkyLight(*this);
}

void SkyLightComponent::OnEndPlay()
{
    IRenderer::Get().RemoveSkyLight(*this);
}
}
}
