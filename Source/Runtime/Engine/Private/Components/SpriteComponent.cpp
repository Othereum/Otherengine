#include "Components/SpriteComponent.hpp"
#include "Actors/Actor.hpp"
#include "Engine/Texture.hpp"

namespace oeng
{
inline namespace engine
{
SpriteComponent::SpriteComponent(AActor& owner, const int draw_order, const int update_order)
    : SceneComponent{owner, update_order},
      texture_{Texture::GetDefault()},
      draw_order_{draw_order}
{
}

SpriteComponent::~SpriteComponent()
{
    if (HasBegunPlay())
        GetRenderer().UnregisterSprite(*this);
}

void SpriteComponent::OnBeginPlay()
{
    GetRenderer().RegisterSprite(*this);
}
}
