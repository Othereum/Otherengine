#include "renderer.h"
#include <stdexcept>
#include <SDL.h>
#include <SDL_image.h>
#include "components/sprite_component.h"

namespace game
{
	static renderer_ptr create_renderer(SDL_Window& window)
	{
		renderer_ptr renderer{
			SDL_CreateRenderer(&window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC),
			SDL_DestroyRenderer
		};
		if (!renderer) throw std::runtime_error{SDL_GetError()};
		return renderer;
	}

	renderer::renderer(SDL_Window& window)
		:renderer_{create_renderer(window)}
	{
	}

	void renderer::draw()
	{
		SDL_RenderClear(renderer_.get());

		for (auto& sprite : sprites_)
		{
			sprite.get().draw(*this);
		}

		SDL_RenderPresent(renderer_.get());
	}

	void renderer::draw(SDL_Texture& texture, const frect& dest, degrees angle) const
	{
		const SDL_Rect r = dest;
		SDL_RenderCopyEx(renderer_.get(), &texture, nullptr, &r, angle.get(), nullptr, SDL_FLIP_NONE);
	}

	void renderer::draw(SDL_Texture& texture, const SDL_Rect& src, const SDL_Rect& dest, degrees angle) const
	{
		SDL_RenderCopyEx(renderer_.get(), &texture, &src, &dest, angle.get(), nullptr, SDL_FLIP_NONE);
	}

	void renderer::add_sprite(const sprite_component& sprite)
	{
		auto cmp = [](const sprite_component& a, const sprite_component& b)
		{
			return a.get_draw_order() <= b.get_draw_order();
		};
		const auto pos = std::lower_bound(sprites_.begin(), sprites_.end(), sprite, cmp);
		sprites_.emplace(pos, sprite);
	}

	void renderer::remove_sprite(const sprite_component& sprite)
	{
		auto pr = [&](const sprite_component& v) { return &v == &sprite; };
		const auto found = std::find_if(sprites_.crbegin(), sprites_.crend(), pr);
		if (found != sprites_.crend()) sprites_.erase(found.base() - 1);
	}
	
	std::shared_ptr<SDL_Texture> renderer::load_texture(const char* const filename)
	{
		const std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)> surface{IMG_Load(filename), SDL_FreeSurface};
		if (!surface) throw std::runtime_error{SDL_GetError()};

		auto deleter = [this, filename](SDL_Texture* const texture)
		{
			textures_.erase(filename);
			SDL_DestroyTexture(texture);
		};
		std::shared_ptr<SDL_Texture> texture{SDL_CreateTextureFromSurface(renderer_.get(), surface.get()), std::move(deleter)};
		if (!texture) throw std::runtime_error{SDL_GetError()};

		return texture;
	}

	std::shared_ptr<SDL_Texture> renderer::get_texture(const char* const filename)
	{
		const auto found = textures_.find(filename);
		if (found != textures_.end()) return found->second.lock();

		const auto loaded = load_texture(filename);
		textures_.emplace(filename, loaded);

		return loaded;
	}
}
