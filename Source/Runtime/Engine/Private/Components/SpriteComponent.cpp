#include "Components/SpriteComponent.hpp"
#include "IRenderer.hpp"
#include "Engine/Texture.hpp"

namespace oeng
{
inline namespace engine
{
SpriteComponent::SpriteComponent()
    : texture_{Texture::GetDefault()}
{
}

void SpriteComponent::SetDrawOrder(int draw_order)
{
    draw_order_ = draw_order;

    if (HasBegunPlay())
    {
        IRenderer::Get().RemoveSprite(*this);
        IRenderer::Get().AddSprite(*this);
    }
}

void SpriteComponent::OnBeginPlay()
{
    IRenderer::Get().AddSprite(*this);
}

void SpriteComponent::OnEndPlay()
{
    IRenderer::Get().RemoveSprite(*this);
}
}
}
