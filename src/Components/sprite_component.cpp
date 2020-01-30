#include "components/sprite_component.h"
#include <SDL.h>
#include "actors/Actor.h"
#include "game.h"

NEG_BEGIN

sprite_component::sprite_component(actor& owner, const int draw_order, const int update_order)
	:component{owner, update_order}, draw_order_{draw_order}
{
	owner.game.add_sprite(*this);
}

sprite_component::~sprite_component()
{
	owner.game.remove_sprite(*this);
}

void sprite_component::draw(SDL_Renderer* const renderer) const
{
	if (!texture_) return;

	const vector2<int> size = tex_size_ * owner.get_scale();
	const vector2<int> pos = owner.get_pos() - size/2;

	const SDL_Rect rect{pos.x, pos.y, size.x, size.y};
	SDL_RenderCopyEx(renderer, texture_.get(), nullptr, &rect, owner.get_rot(), nullptr, SDL_FLIP_NONE);
}

void sprite_component::set_texture(std::shared_ptr<SDL_Texture>&& texture)
{
	texture_ = std::move(texture);

	int w, h;
	SDL_QueryTexture(texture_.get(), nullptr, nullptr, &w, &h);
	tex_size_.reset(w, h);
}

NEG_END