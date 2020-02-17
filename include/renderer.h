#pragma once

#include "vector.h"
#include "rotation.h"
#include <memory>

struct SDL_Window;
struct SDL_Texture;
struct SDL_Renderer;

namespace game
{
	using renderer_ptr = std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)>;
	
	class renderer
	{
	public:
		explicit renderer(SDL_Window& window);
		void draw(SDL_Texture& texture, vector2<int> pos, vector2<int> size, degrees angle);

	private:
		renderer_ptr renderer_;
	};
}
