#pragma once
#include <chrono>
#include <memory>
#include <unordered_map>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;

namespace game
{
	constexpr auto screen_w = 1024;
	constexpr auto screen_h = 768;

	using window_ptr = std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>;
	using renderer_ptr = std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)>;
	
	class sprite_component;
	class world;

	class application
	{
	public:
		application();
		~application();
		application(const application&) = delete;
		application(application&&) = delete;
		application& operator=(const application&) = delete;
		application& operator=(application&&) = delete;
		
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

		struct sdl_raii
		{
			sdl_raii();
			~sdl_raii();
			sdl_raii(const sdl_raii&) = delete;
			sdl_raii(sdl_raii&&) = delete;
			sdl_raii& operator=(const sdl_raii&) = delete;
			sdl_raii& operator=(sdl_raii&&) = delete;
		} sdl_raii_;

		uint8_t refresh_rate_;
		bool is_running_ = true;
		
		std::chrono::time_point<std::chrono::steady_clock> time_;
		
		window_ptr window_;
		renderer_ptr renderer_;

		std::vector<std::reference_wrapper<const sprite_component>> sprites_;
		std::unordered_map<std::string, std::weak_ptr<SDL_Texture>> textures_;

		std::unique_ptr<world> world_;
	};
}