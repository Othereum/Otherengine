#include "components/bg_sprite_component.h"
#include <SDL.h>
#include "application.h"
#include "actors/actor.h"

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
			bg.offset.x += scroll_speed_ * delta_seconds;
			if (bg.offset.x < -scrsz.x)
			{
				bg.offset.x = static_cast<float>((textures_.size() - 1) * scrsz.x - 1);
			}
		}
	}

	void bg_sprite_component::draw() const
	{
		for (const auto& bg : textures_)
		{
			get_app().draw(*bg.texture, {get_owner().get_pos() + bg.offset, fvector2{scrsz}}, 0_deg);
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
			texture.offset.x = static_cast<float>(i * scrsz.x);
			textures_.push_back(std::move(texture));
		}
		textures.clear();
	}
}
