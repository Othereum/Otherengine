#include "Components/SpriteComponent.hpp"
#include "Actors/Actor.hpp"
#include "Engine.hpp"
#include "Renderer.hpp"

namespace oeng
{
	SpriteComponent::SpriteComponent(AActor& owner, const int draw_order, const int update_order)
		:SceneComponent{owner, update_order},
		texture_{GetRenderer().GetDefaultTexture()},
		draw_order_{draw_order}
	{
	}

	SpriteComponent::~SpriteComponent()
	{
		if (HasBegunPlay()) GetRenderer().UnregisterSprite(*this);
	}

	void SpriteComponent::SetTexture(Path file)
	{
		SetTexture(GetRenderer().GetTexture(file));
	}

	void SpriteComponent::OnBeginPlay()
	{
		GetRenderer().RegisterSprite(*this);
	}
}
