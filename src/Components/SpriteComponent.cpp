#include <Components/SpriteComponent.h>
#include <SDL.h>
#include <Actors/Actor.h>
#include <Game.h>

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

	SDL_Rect rect;
	rect.w = tex_w_ * owner.get_scale();
	rect.h = tex_h_ * owner.get_scale();
	rect.x = owner.get_pos().x - rect.w/2;
	rect.y = owner.get_pos().y - rect.h/2;

	SDL_RenderCopyEx(renderer, texture_.get(), nullptr, &rect, owner.get_rot(), nullptr, SDL_FLIP_NONE);
}

void sprite_component::set_texture(std::shared_ptr<SDL_Texture>&& texture)
{
	texture_ = std::move(texture);

	int w, h;
	SDL_QueryTexture(texture_.get(), nullptr, nullptr, &w, &h);
	tex_w_ = w;
	tex_h_ = h;
}

NEG_END