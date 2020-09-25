#include "Components/SkyLightComponent.hpp"
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
} // namespace engine
} // namespace oeng
