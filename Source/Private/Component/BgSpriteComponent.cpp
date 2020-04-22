#include "Components/BgSpriteComponent.h"
#include <SDL.h>
#include "Actor.h"
#include "Graphics/Renderer.h"

namespace oeng
{
	CBgSpriteComponent::CBgSpriteComponent(AActor& owner, int draw_order, int update_order)
		:CSpriteComponent{owner, draw_order, update_order}
	{
	}

	void CBgSpriteComponent::Update(const float delta_seconds)
	{
		CSpriteComponent::Update(delta_seconds);

		for (auto& bg : textures_)
		{
			bg.offset.x += scroll_speed_ * delta_seconds;
			if (bg.offset.x < -graphics::kScrSz.x)
			{
				bg.offset.x = static_cast<float>((textures_.size() - 1) * graphics::kScrSz.x - 1);
			}
		}
	}

	void CBgSpriteComponent::Draw(const graphics::CRenderer& renderer) const
	{
		for (const auto& bg : textures_)
		{
			renderer.Draw(math::Scale(graphics::kScrSz) * math::Translation(GetOwner().GetPos() + bg.offset));
		}
	}

	void CBgSpriteComponent::SetBgTextures(std::vector<std::shared_ptr<SDL_Texture>>&& textures)
	{
		textures_.clear();
		AddBgTextures(std::move(textures));
	}

	void CBgSpriteComponent::AddBgTextures(std::vector<std::shared_ptr<SDL_Texture>>&& textures)
	{
		for (size_t i = 0; i < textures.size(); ++i)
		{
			FBgTexture texture;
			texture.texture = std::move(textures[i]);
			texture.offset.x = static_cast<float>(i * graphics::kScrSz.x);
			textures_.push_back(std::move(texture));
		}
		textures.clear();
	}
}
