#pragma once
#include "components/sprite_component.h"
#include <vector>

namespace game
{
	class anim_sprite_component : public sprite_component
	{
	public:
		explicit anim_sprite_component(actor& owner, int draw_order = 100, int update_order = 100);
		void set_anim_textures(std::vector<std::shared_ptr<SDL_Texture>>&& textures);
		
		void set_anim_fps(float fps) { anim_fps_ = fps; }
		[[nodiscard]] float get_anim_fps() const { return anim_fps_; }

	private:
		void update(float delta_seconds) override;
		
		float cur_frame_{};
		float anim_fps_{};
		
		std::vector<std::shared_ptr<SDL_Texture>> anim_textures_;
	};
}
