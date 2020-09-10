#include "Components/SpriteComponent.hpp"
#include "Engine/Engine.hpp"
#include "Engine/Texture.hpp"
#include "IRenderer.hpp"

namespace oeng
{
inline namespace engine
{

SpriteComponent::SpriteComponent() : texture_{Texture::GetDefault()}
{
}

void SpriteComponent::SetDrawOrder(int draw_order)
{
    draw_order_ = draw_order;

    if (HasBegunPlay())
    {
        auto& renderer = GetEngine().GetRenderer();
        renderer.RemoveSprite(*this);
        renderer.AddSprite(*this);
    }
}

void SpriteComponent::OnBeginPlay()
{
    GetEngine().GetRenderer().AddSprite(*this);
}

void SpriteComponent::OnEndPlay()
{
    GetEngine().GetRenderer().RemoveSprite(*this);
}

} // namespace engine
} // namespace oeng
