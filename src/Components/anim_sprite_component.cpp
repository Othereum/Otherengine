#include "components/anim_sprite_component.h"
#include <cmath>
#include "math_utils.h"

namespace game
{
	anim_sprite_component::animation::animation(std::initializer_list<std::shared_ptr<SDL_Texture>> textures, float fps, bool loop, float start_frame)
		:loop{loop}, cur{start_frame}, fps{fps}, textures{textures}
	{
	}

	anim_sprite_component::anim_sprite_component(actor& owner, int draw_order, int update_order)
		:sprite_component{owner, draw_order, update_order}
	{
	}

	size_t anim_sprite_component::add_anim(animation&& anim)
	{
		anims_.push_back(std::move(anim));
		return anims_.size() - 1;
	}

	void anim_sprite_component::update(const float delta_seconds)
	{
		auto& anim = anims_[idx_];
		const auto old_frame = static_cast<size_t>(anim.cur);
		anim.cur += anim.fps * delta_seconds;
		anim.cur = anim.loop ? fmod(anim.cur, static_cast<float>(anim.textures.size())) : math::min(anim.cur, anim.textures.size() - 1);
		const auto new_frame = static_cast<size_t>(anim.cur);
		if (old_frame != new_frame) set_texture(anim.textures[new_frame]);
	}
}
