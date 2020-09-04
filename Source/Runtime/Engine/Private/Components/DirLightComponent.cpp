#include "Components/DirLightComponent.hpp"
#include "IRenderer.hpp"

namespace oeng
{
inline namespace engine
{
void DirLightComponent::OnBeginPlay()
{
    IRenderer::Get().AddDirLight(*this);
}

void DirLightComponent::OnEndPlay()
{
    IRenderer::Get().RemoveDirLight(*this);
}
}
}
