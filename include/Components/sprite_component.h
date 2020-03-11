#pragma once

#include <memory>
#include "components/component.h"
#include "vector.h"

struct SDL_Renderer;
struct SDL_Texture;

namespace game
{
	class renderer;
	
	class sprite_component : public component
	{
	public:
		explicit sprite_component(actor& owner, int draw_order = 100, int update_order = 100);
		~sprite_component();

		sprite_component(const sprite_component&) = delete;
		sprite_component(sprite_component&&) = delete;
		sprite_component& operator=(const sprite_component&) = delete;
		sprite_component& operator=(sprite_component&&) = delete;

		virtual void draw(renderer& renderer) const;
		
		void set_texture(std::shared_ptr<SDL_Texture>&& texture);
		void set_texture(const std::shared_ptr<SDL_Texture>& texture);
		void set_texture(const char* filename);
		[[nodiscard]] SDL_Texture& get_texture() const { return *texture_; }

		[[nodiscard]] const vector2<unsigned short>& get_tex_size() const noexcept { return tex_size_; }
		[[nodiscard]] int get_draw_order() const noexcept { return draw_order_; }
		[[nodiscard]] renderer& get_renderer() const noexcept;

	private:
		vector2<uint16_t> tex_size_;
		int draw_order_;
		std::shared_ptr<SDL_Texture> texture_;
	};
}