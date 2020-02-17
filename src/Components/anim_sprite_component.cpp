#include "components/anim_sprite_component.h"
#include <cmath>

namespace game
{
	anim_sprite_component::anim_sprite_component(actor& owner, int draw_order, int update_order)
		:sprite_component{owner, draw_order, update_order}
	{
	}

	void anim_sprite_component::set_anim_textures(std::vector<std::shared_ptr<SDL_Texture>>&& textures)
	{
		anim_textures_ = std::move(textures);
	}

	void anim_sprite_component::update(const float delta_seconds)
	{
		sprite_component::update(delta_seconds);

		if (!anim_textures_.empty())
		{
			const auto old_frame = static_cast<size_t>(cur_frame_);
			cur_frame_ += get_anim_fps() * delta_seconds;
			cur_frame_ = fmod(cur_frame_, static_cast<float>(anim_textures_.size()));
			const auto new_frame = static_cast<size_t>(cur_frame_);
			if (old_frame != new_frame)
			{
				auto texture = anim_textures_[new_frame];
				set_texture(std::move(texture));
			}
		}
	}
}
