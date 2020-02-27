#pragma once
#include "components/sprite_component.h"
#include <vector>

namespace game
{
	class anim_sprite_component : public sprite_component
	{
	public:
		struct animation
		{
			animation(std::initializer_list<std::shared_ptr<SDL_Texture>> textures, float fps = 24, bool loop = true, float start_frame = 0);
			
			bool loop;
			float cur;
			float fps;
			std::vector<std::shared_ptr<SDL_Texture>> textures;
		};
		
		explicit anim_sprite_component(actor& owner, int draw_order = 100, int update_order = 100);
		
		size_t add_anim(animation&& anim);
		void play(size_t idx, bool reset = true) { idx_ = idx; if (reset) play(); }
		void play() { anims_[idx_].cur = 0; }

	private:
		void update(float delta_seconds) override;

		size_t idx_ = 0;
		std::vector<animation> anims_;
	};
}
