#include "Components/SpriteComponent.h"
#include <SDL.h>
#include "Actor.h"
#include "Engine.h"
#include "World.h"
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

	void CSpriteComponent::DrawSprite() const
	{
		if (IsEnabled())
		{
			Draw();
		}
	}

	void CSpriteComponent::Draw() const
	{
		// if (!texture_) return;
		
		auto& owner = GetOwner();
		GetRenderer().Draw(*texture_, {owner.GetPos(), tex_size_ * owner.GetScale()}, owner.GetRot());
	}

	void CSpriteComponent::SetTexture(std::shared_ptr<SDL_Texture>&& texture)
	{
		if (!texture) return;
		texture_ = std::move(texture);

		int w, h;
		SDL_QueryTexture(texture_.get(), nullptr, nullptr, &w, &h);
		tex_size_ = Vector<uint16_t, 2>{w, h};
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

	graphics::CRenderer& CSpriteComponent::GetRenderer() const noexcept
	{
		return GetWorld().GetRenderer();
	}
}
