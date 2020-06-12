#include "Components/SpriteComponent.hpp"
#include "GameFramework/Actor.hpp"
#include "Engine.hpp"
#include "Renderer.hpp"

namespace oeng
{
	SpriteComponent::SpriteComponent(AActor& owner, const int draw_order, const int update_order)
		:SceneComponent{owner, update_order}, draw_order_{draw_order}
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

	Renderer& SpriteComponent::GetRenderer() const noexcept
	{
		return GetEngine().GetRenderer();
	}

	void SpriteComponent::OnBeginPlay()
	{
		GetRenderer().RegisterSprite(*this);
	}
}
