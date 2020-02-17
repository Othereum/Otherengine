#include "components/bg_sprite_component.h"
#include <SDL.h>
#include "application.h"

namespace game
{
	bg_sprite_component::bg_sprite_component(actor& owner, int draw_order, int update_order)
		:sprite_component{owner, draw_order, update_order}
	{
	}

	void bg_sprite_component::update(const float delta_seconds)
	{
		sprite_component::update(delta_seconds);

		for (auto& bg : textures_)
		{
			bg.offset.x -= scroll_speed_ * delta_seconds;
			if (bg.offset.x < -screen.x)
			{
				bg.offset.x = (textures_.size() - 1) * screen.x - 1;
			}
		}
	}

	void bg_sprite_component::draw(SDL_Renderer& renderer) const
	{
		for (const auto& bg : textures_)
		{
			const auto pos = bg.offset - screen / 2;
			const SDL_Rect rect{pos.x, pos.y, screen.x, screen.y};
			SDL_RenderCopyEx(&renderer, bg.texture.get(), nullptr, &rect, 0, nullptr, SDL_FLIP_NONE);
		}
	}

	void bg_sprite_component::set_bg_textures(std::vector<std::shared_ptr<SDL_Texture>>&& textures)
	{
		textures_.clear();
		add_bg_textures(std::move(textures));
	}

	void bg_sprite_component::add_bg_textures(std::vector<std::shared_ptr<SDL_Texture>>&& textures)
	{
		for (size_t i = 0; i < textures.size(); ++i)
		{
			bg_texture texture;
			texture.texture = std::move(textures[i]);
			texture.offset.x = i * screen.x;
			textures_.push_back(std::move(texture));
		}
		textures.clear();
	}
}
