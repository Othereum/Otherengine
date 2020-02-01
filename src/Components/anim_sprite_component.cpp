#include "components/anim_sprite_component.h"
#include <cmath>

namespace game
{
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
