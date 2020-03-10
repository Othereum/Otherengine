#include "application.h"
#include <stdexcept>
#include <SDL.h>
#include <SDL_image.h>
#include "actors/actor.h"
#include "world.h"
#include "components/tilemap_component.h"
#include "components/input_component.h"

namespace game
{
	static window_ptr create_window()
	{
		window_ptr window{
			SDL_CreateWindow(PROJECT_NAME, 100, 100, scrsz.x, scrsz.y, 0),
			SDL_DestroyWindow
		};
		if (!window) throw std::runtime_error{SDL_GetError()};
		return window;
	}

	static uint16_t get_refresh_rate(SDL_Window& window)
	{
		SDL_DisplayMode mode;
		if (SDL_GetWindowDisplayMode(&window, &mode) != 0) throw std::runtime_error{SDL_GetError()};
		return mode.refresh_rate;
	}

	application::application():
		window_{create_window()},
		renderer_{*window_},
		world_{std::make_unique<world>(*this)},
		refresh_rate_{get_refresh_rate(*window_)}
	{
		load_data();
	}

	application::~application() = default;

	void application::run_loop()
	{
		while (is_running_)
		{
			process_input();
			update_game();
			generate_output();
		}
	}

	void application::shutdown()
	{
		is_running_ = false;
	}

	void application::register_input_component(const input_component& comp)
	{
		auto cmp = [](const input_component& a, const input_component& b)
		{
			return a.get_receive_order() <= b.get_receive_order();
		};
		const auto pos = std::lower_bound(input_comps_.begin(), input_comps_.end(), comp, cmp);
		input_comps_.emplace(pos, comp);
	}

	void application::unregister_input_component(const input_component& comp)
	{
		auto pr = [&](const input_component& v) { return &v == &comp; };
		const auto found = std::find_if(input_comps_.crbegin(), input_comps_.crend(), pr);
		if (found != input_comps_.crend()) input_comps_.erase(found.base() - 1);
	}

	void application::load_data()
	{
		auto& temp = world_->spawn_actor<actor>();
		temp.set_pos({scrsz.x/2.f, scrsz.y/2.f});

		auto& bg1 = temp.add_component<tilemap_component>(101);
		bg1.set_texture(renderer_.get_texture("Assets/Tiles.png"));
		bg1.load_tile_from_csv("Assets/MapLayer1.csv");

		auto& bg2 = temp.add_component<tilemap_component>();
		bg2.set_texture(renderer_.get_texture("Assets/Tiles.png"));
		bg2.load_tile_from_csv("Assets/MapLayer2.csv");

		auto& bg3 = temp.add_component<tilemap_component>(9);
		bg3.set_texture(renderer_.get_texture("Assets/Tiles.png"));
		bg3.load_tile_from_csv("Assets/MapLayer3.csv");
	}

	void application::process_input()
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)
			{
				shutdown();
				return;
			}
		}

		const auto keyboard = SDL_GetKeyboardState(nullptr);
		if (keyboard[SDL_SCANCODE_ESCAPE]) shutdown();
	}

	void application::update_game()
	{
		const auto delta_seconds = update_time();
		world_->update(delta_seconds);
	}

	void application::generate_output()
	{
		renderer_.draw();
	}

	float application::update_time()
	{
		using namespace std::chrono;
		const auto now = steady_clock::now();
		const auto delta_seconds = duration<float>{now - time_}.count();
		time_ = now;
		return delta_seconds;
	}

	sdl_raii::sdl_raii()
	{
		const auto sdl_result = SDL_Init(SDL_INIT_VIDEO);
		if (sdl_result != 0) throw std::runtime_error{SDL_GetError()};

		const auto flags = IMG_INIT_PNG;
		const auto img_result = IMG_Init(flags);
		if (img_result != flags) throw std::runtime_error{IMG_GetError()};
	}

	sdl_raii::~sdl_raii()
	{
		IMG_Quit();
		SDL_Quit();
	}
}
