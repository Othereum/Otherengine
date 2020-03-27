#include "Renderer.h"
#include <stdexcept>
#include <SDL.h>
#include "Component/SpriteComponent.h"

namespace game
{
	static CRenderer::TWindowPtr CreateWindow()
	{
		const auto window = SDL_CreateWindow(PROJECT_NAME, 100, 100, kScrSz.x, kScrSz.y, SDL_WINDOW_OPENGL);
		if (!window) throw std::runtime_error{SDL_GetError()};
		return {window, &SDL_DestroyWindow};
	}

	static CRenderer::TRendererPtr CreateRenderer(SDL_Window& window)
	{
		const auto renderer = SDL_CreateRenderer(&window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (!renderer) throw std::runtime_error{SDL_GetError()};
		return {renderer, &SDL_DestroyRenderer};
	}
	
	CRenderer::CRenderer()
		:window_{CreateWindow()}, renderer_{CreateRenderer(*window_)}
	{
	}

	void CRenderer::RegisterSprite(const CSpriteComponent& sprite)
	{
		auto cmp = [](const CSpriteComponent& a, const CSpriteComponent& b)
		{
			return a.GetDrawOrder() <= b.GetDrawOrder();
		};
		const auto pos = std::lower_bound(sprites_.begin(), sprites_.end(), sprite, cmp);
		sprites_.emplace(pos, sprite);
	}

	void CRenderer::UnregisterSprite(const CSpriteComponent& sprite)
	{
		auto pr = [&](const CSpriteComponent& v) { return &v == &sprite; };
		const auto found = std::find_if(sprites_.crbegin(), sprites_.crend(), pr);
		if (found != sprites_.crend()) sprites_.erase(found.base() - 1);
	}

	void CRenderer::Draw(SDL_Texture& texture, const TFRect& dest, Degrees angle) const
	{
		const SDL_Rect r = dest;
		SDL_RenderCopyEx(renderer_.get(), &texture, nullptr, &r, angle.Get(), nullptr, SDL_FLIP_NONE);
	}

	void CRenderer::Draw(SDL_Texture& texture, const SDL_Rect& src, const SDL_Rect& dest, Degrees angle) const
	{
		SDL_RenderCopyEx(renderer_.get(), &texture, &src, &dest, angle.Get(), nullptr, SDL_FLIP_NONE);
	}

	void CRenderer::DrawScene() const
	{
		SDL_RenderClear(renderer_.get());

		for (auto& sprite : sprites_)
		{
			sprite.get().DrawSprite();
		}

		SDL_RenderPresent(renderer_.get());
	}
}
