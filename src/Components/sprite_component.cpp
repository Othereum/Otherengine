#include "components/sprite_component.h"
#include <SDL.h>
#include "actors/actor.h"
#include "application.h"
#include "renderer.h"

namespace game
{
	sprite_component::sprite_component(actor& owner, const int draw_order, const int update_order)
		:component{owner, update_order}, draw_order_{draw_order}
	{
		get_renderer().add_sprite(*this);
	}

	sprite_component::~sprite_component()
	{
		get_renderer().remove_sprite(*this);
	}

	void sprite_component::draw(renderer& renderer) const
	{
		if (!texture_) return;
		
		auto& owner = get_owner();
		renderer.draw(*texture_, {owner.get_pos(), tex_size_ * owner.get_scale()}, owner.get_rot());
	}

	void sprite_component::set_texture(std::shared_ptr<SDL_Texture>&& texture)
	{
		texture_ = std::move(texture);

		int w, h;
		SDL_QueryTexture(texture_.get(), nullptr, nullptr, &w, &h);
		tex_size_ = vector2{uint16_t(w), uint16_t(h)};
	}

	void sprite_component::set_texture(const std::shared_ptr<SDL_Texture>& texture)
	{
		auto temp = texture;
		set_texture(std::move(temp));
	}

	void sprite_component::set_texture(const char* filename)
	{
		set_texture(get_renderer().get_texture(filename));
	}

	renderer& sprite_component::get_renderer() const noexcept
	{
		return get_app().get_renderer();
	}
}
