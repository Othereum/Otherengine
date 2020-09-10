#include "Components/DirLightComponent.hpp"
#include "IRenderer.hpp"

namespace oeng
{
inline namespace engine
{

void DirLightComponent::OnBeginPlay()
{
    GetRenderer().AddDirLight(*this);
}

void DirLightComponent::OnEndPlay()
{
    GetRenderer().RemoveDirLight(*this);
}

} // namespace engine
} // namespace oeng
