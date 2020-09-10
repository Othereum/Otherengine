#include "Components/PointLightComponent.hpp"
#include "IRenderer.hpp"

namespace oeng
{
inline namespace engine
{

void PointLightComponent::OnBeginPlay()
{
    GetRenderer().AddPointLight(*this);
}

void PointLightComponent::OnEndPlay()
{
    GetRenderer().RemovePointLight(*this);
}

} // namespace engine
} // namespace oeng
