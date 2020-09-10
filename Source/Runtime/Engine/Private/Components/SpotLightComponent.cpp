#include "Components/SpotLightComponent.hpp"
#include "IRenderer.hpp"

namespace oeng
{
inline namespace engine
{

void SpotLightComponent::OnBeginPlay()
{
    GetRenderer().AddSpotLight(*this);
}

void SpotLightComponent::OnEndPlay()
{
    GetRenderer().RemoveSpotLight(*this);
}

} // namespace engine
} // namespace oeng
