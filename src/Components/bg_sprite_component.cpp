#include "components/bg_sprite_component.h"

namespace game
{
	bg_sprite_component::bg_sprite_component(actor& owner, int draw_order, int update_order)
		:sprite_component{owner, draw_order, update_order}
	{
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
			texture.offset.x = i * screen_size_.x;
			textures_.push_back(std::move(texture));
		}
		textures.clear();
	}
}
