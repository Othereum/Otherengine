#include "application.h"
#include <thread>
#include <stdexcept>
#include <SDL.h>
#include <SDL_image.h>
#include "actors/actor.h"
#include "math_utils.h"
#include "world.h"
#include "actors/ship.h"
#include "components/tilemap_component.h"

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

	void application::load_data()
	{
		ship_ = &world_->spawn_actor<ship>();
		ship_->set_pos({100, 384});
		ship_->set_scale(1.5f);

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
			if (event.type == SDL_QUIT)
			{
				shutdown();
				return;
			}
		}

		const auto keyboard = SDL_GetKeyboardState(nullptr);
		if (keyboard[SDL_SCANCODE_ESCAPE]) shutdown();

		ship_->process_keyboard(keyboard);
	}

	void application::update_game()
	{
		using namespace std::chrono;

		constexpr nanoseconds sec = 1s;
		constexpr auto min_fps = 10;
		constexpr auto time_speed = 1;

		std::this_thread::sleep_until(time_ + sec / refresh_rate_);

		const auto now = steady_clock::now();
		const auto delta_seconds = duration<float>{math::min(now - time_, sec / min_fps) * time_speed}.count();
		time_ = now;

		world_->update(delta_seconds);
	}

	void application::generate_output()
	{
		renderer_.draw();
	}

	sdl_raii::sdl_raii()
	{
		const auto sdl_result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
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
