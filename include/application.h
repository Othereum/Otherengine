#pragma once

#include <chrono>
#include <memory>
#include <unordered_map>

#include "rotation.h"
#include "vector.h"
#include "rect.h"

struct SDL_Window;
struct SDL_Texture;
struct SDL_Renderer;
struct SDL_Rect;

namespace game
{
	class sprite_component;
	constexpr vector2<uint16_t> scrsz{1024, 768};

	using window_ptr = std::unique_ptr<SDL_Window, void(*)(SDL_Window*)>;
	using renderer_ptr = std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)>;
	
	class actor;
	class input_component;
	class circle_component;

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

		
		template <class T>
		T& spawn_actor()
		{
			static_assert(std::is_base_of_v<actor, T>);
			auto ptr = std::make_unique<T>(*this);
			auto& actor = *ptr;
			register_actor(std::move(ptr));
			return actor;
		}
		
		void register_input_component(const input_component& comp);
		void unregister_input_component(const input_component& comp);

		void register_circle_component(circle_component& comp);
		void unregister_circle_component(circle_component& comp);

		void register_sprite(const sprite_component& sprite);
		void unregister_sprite(const sprite_component& sprite);

		
		void draw(SDL_Texture& texture, const frect& dest, degrees angle = {}) const;
		void draw(SDL_Texture& texture, const SDL_Rect& src, const SDL_Rect& dest, degrees angle = {}) const;
		
		[[nodiscard]] std::shared_ptr<SDL_Texture> get_texture(const char* filename);

		
	private:
		void process_input();
		void update_game();
		float update_time();
		void generate_output();
		
		void register_actor(std::unique_ptr<actor>&& actor);
		
		[[nodiscard]] std::shared_ptr<SDL_Texture> load_texture(const char* filename);

		
		window_ptr window_;
		renderer_ptr renderer_;

		std::unordered_map<std::string, std::weak_ptr<SDL_Texture>> textures_;
		
		std::vector<std::reference_wrapper<const sprite_component>> sprites_;
		std::vector<std::reference_wrapper<const input_component>> input_comps_;
		std::vector<std::reference_wrapper<circle_component>> circle_comps_;
		
		std::vector<std::unique_ptr<actor>> actors_;
		std::vector<std::unique_ptr<actor>> pending_actors_;
		
		std::chrono::time_point<std::chrono::steady_clock> time_;

		bool is_updating_actors_ : 1;
		bool is_running_ : 1;
	};
}