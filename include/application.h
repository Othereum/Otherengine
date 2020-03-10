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
	constexpr vector2<uint16_t> scrsz{1024, 768};

	using window_ptr = std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>;
	
	class input_component;
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

		void register_input_component(const input_component& comp);
		void unregister_input_component(const input_component& comp);

		[[nodiscard]] renderer& get_renderer() { return renderer_; }

	private:
		void load_data();
		
		void process_input();
		void update_game();
		void generate_output();

		float update_time();
		
		window_ptr window_;
		renderer renderer_;

		std::unique_ptr<world> world_;
		std::vector<std::reference_wrapper<const input_component>> input_comps_;
		
		std::chrono::time_point<std::chrono::steady_clock> time_;
		uint16_t refresh_rate_;
		bool is_running_ = true;
	};
}