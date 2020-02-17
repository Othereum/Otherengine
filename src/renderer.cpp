#include "renderer.h"
#include <SDL.h>
#include <stdexcept>

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

	void renderer::draw(SDL_Texture& texture, vector2<int> pos, vector2<int> size, degrees angle)
	{
	}
}
