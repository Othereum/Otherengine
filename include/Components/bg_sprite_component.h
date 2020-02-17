#pragma once
#include "sprite_component.h"
#include <vector>

namespace game
{
	class bg_sprite_component : public sprite_component
	{
	public:
		explicit bg_sprite_component(actor& owner, int draw_order = 10, int update_order = 100);
		
		void update(float delta_seconds) override;
		void draw(SDL_Renderer& renderer) const override;

		void set_bg_textures(std::vector<std::shared_ptr<SDL_Texture>>&& textures);
		void add_bg_textures(std::vector<std::shared_ptr<SDL_Texture>>&& textures);

		void set_scroll_speed(float speed) { scroll_speed_ = speed; }
		[[nodiscard]] float get_scroll_speed() const { return scroll_speed_; }

	private:
		struct bg_texture
		{
			std::shared_ptr<SDL_Texture> texture;
			vector2<int> offset;
		};
		std::vector<bg_texture> textures_;
		float scroll_speed_{};
	};
}
