#include "Components/SpriteComponent.h"
#include "Actor.h"
#include "Engine.h"
#include "Graphics/Renderer.h"

namespace oeng
{
	CSpriteComponent::CSpriteComponent(AActor& owner, const int draw_order, const int update_order)
		:CActorComponent{owner, update_order}, draw_order_{draw_order}
	{
	}

	CSpriteComponent::~CSpriteComponent()
	{
		GetRenderer().UnregisterSprite(*this);
	}

	void CSpriteComponent::BeginPlay()
	{
		GetRenderer().RegisterSprite(*this);
	}

	void CSpriteComponent::SetTexture(std::shared_ptr<graphics::Texture>&& texture)
	{
		texture_ = std::move(texture);
	}

	void CSpriteComponent::SetTexture(const std::shared_ptr<graphics::Texture>& texture)
	{
		texture_ = texture;
	}

	void CSpriteComponent::SetTexture(const char* filename)
	{
		SetTexture(GetEngine().GetTexture(filename));
	}

	graphics::CRenderer& CSpriteComponent::GetRenderer() const noexcept
	{
		return GetEngine().GetRenderer();
	}
}
