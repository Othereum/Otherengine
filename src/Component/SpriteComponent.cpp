#include "Component/SpriteComponent.h"
#include <SDL.h>
#include "Actor/Actor.h"
#include "Engine.h"
#include "World.h"
#include "Graphic/Renderer.h"

namespace game
{
	CSpriteComponent::CSpriteComponent(AActor& owner, const int drawOrder, const int updateOrder)
		:CActorComponent{owner, updateOrder}, drawOrder_{drawOrder}
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

	void CSpriteComponent::DrawSprite() const
	{
		if (IsEnabled())
		{
			Draw();
		}
	}

	void CSpriteComponent::Draw() const
	{
		if (!texture_) return;
		
		auto& owner = GetOwner();
		GetRenderer().Draw(*texture_, {owner.GetPos(), texSize_ * owner.GetScale()}, owner.GetRot());
	}

	void CSpriteComponent::SetTexture(std::shared_ptr<SDL_Texture>&& texture)
	{
		texture_ = std::move(texture);

		int w, h;
		SDL_QueryTexture(texture_.get(), nullptr, nullptr, &w, &h);
		texSize_ = Vector2{uint16_t(w), uint16_t(h)};
	}

	void CSpriteComponent::SetTexture(const std::shared_ptr<SDL_Texture>& texture)
	{
		auto temp = texture;
		SetTexture(std::move(temp));
	}

	void CSpriteComponent::SetTexture(const char* filename)
	{
		SetTexture(GetEngine().GetTexture(filename));
	}

	CRenderer& CSpriteComponent::GetRenderer() const noexcept
	{
		return GetWorld().GetRenderer();
	}
}
