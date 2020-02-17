#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include "vector.h"
#include "rotation.h"

struct SDL_Window;
struct SDL_Texture;
struct SDL_Renderer;

namespace game
{
	class sprite_component;
	using renderer_ptr = std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)>;
	
	class renderer
	{
	public:
		explicit renderer(SDL_Window& window);

		void draw();
		void draw(SDL_Texture& texture, fvector2 pos, fvector2 size, degrees angle) const;
		
		void add_sprite(const sprite_component& sprite);
		void remove_sprite(const sprite_component& sprite);
		
		std::shared_ptr<SDL_Texture> get_texture(const char* filename);
		
	private:
		std::shared_ptr<SDL_Texture> load_texture(const char* filename);

		std::vector<std::reference_wrapper<const sprite_component>> sprites_;
		std::unordered_map<std::string, std::weak_ptr<SDL_Texture>> textures_;

		renderer_ptr renderer_;
	};
}
