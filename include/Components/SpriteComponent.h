#pragma once

#include <Components/Component.h>
#include <memory>

class SDL_Renderer;
class SDL_Texture;

NEG_BEGIN

class sprite_component : public component
{
public:
	explicit sprite_component(actor& owner, int draw_order = 100, int update_order = 100);
	~sprite_component();

	sprite_component(const sprite_component&) = delete;
	sprite_component(sprite_component&&) = delete;
	sprite_component& operator=(const sprite_component&) = delete;
	sprite_component& operator=(sprite_component&&) = delete;

	virtual void draw(SDL_Renderer& renderer) const;
	virtual void set_texture(std::shared_ptr<SDL_Texture>&& texture);

	[[nodiscard]] unsigned short get_tex_w() const { return tex_w_; }
	[[nodiscard]] unsigned short get_tex_h() const { return tex_h_; }
	[[nodiscard]] int get_draw_order() const { return draw_order_; }

private:
	unsigned short tex_w_{}, tex_h_{};
	int draw_order_;
	std::shared_ptr<SDL_Texture> texture_;
};

NEG_END