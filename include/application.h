#pragma once
#include <chrono>
#include <memory>
#include <unordered_map>
#include "vector.h"
#include "renderer.h"

struct SDL_Window;
struct SDL_Texture;

namespace game
{
	constexpr vector2<uint16_t> screen{1024, 768};

	using window_ptr = std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>;
	
	class sprite_component;
	class world;

	class sdl_raii
	{
	public:
		sdl_raii();
		~sdl_raii();
	};
	
	class application : sdl_raii
	{
	public:
		application();
		~application();
		
		void run_loop();
		void shutdown();

		void add_sprite(const sprite_component& sprite);
		void remove_sprite(const sprite_component& sprite);
		
		std::shared_ptr<SDL_Texture> get_texture(const char* filename);
		
	private:
		void load_data();
		
		void process_input();
		void update_game();
		void generate_output() const;
		
		std::shared_ptr<SDL_Texture> load_texture(const char* filename);

		window_ptr window_;
		renderer renderer_;

		std::vector<std::reference_wrapper<const sprite_component>> sprites_;
		std::unordered_map<std::string, std::weak_ptr<SDL_Texture>> textures_;

		std::unique_ptr<world> world_;
		
		std::chrono::time_point<std::chrono::steady_clock> time_;
		bool is_running_ = true;
		uint8_t refresh_rate_;
	};
}