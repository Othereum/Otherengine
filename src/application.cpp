#include "application.h"

#include <stdexcept>
#include <SDL.h>
#include <SDL_image.h>

#include "world.h"
#include "actors/actor.h"
#include "components/input_component.h"

#include "actors/asteroid.h"
#include "actors/ship.h"
#include "math_utils.h"

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

	application::application():
		window_{create_window()},
		renderer_{*window_},
		world_{std::make_unique<world>(*this)},
		time_{std::chrono::steady_clock::now()}
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
		for (auto i = 0; i < 20; ++i)
		{
			auto& ast = world_->spawn_actor<asteroid>();
			ast.set_pos(math::rand_vec({0, 0}, fvector2{scrsz}));
			ast.set_rot(math::rand_ang());
		}

		auto& sh = world_->spawn_actor<ship>();
		sh.set_pos(fvector2{scrsz / 2});
	}

	void application::process_input()
	{
		std::vector<SDL_Keycode> key_events[2];
		
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				shutdown();
				return;

			case SDL_KEYDOWN: case SDL_KEYUP:
				if (!event.key.repeat)
				{
					key_events[event.key.state].push_back(event.key.keysym.sym);
					break;
				}
			}
		}

		const auto keyboard = SDL_GetKeyboardState(nullptr);
		for (auto comp : input_comps_)
		{
			comp.get().process_input(key_events, keyboard);
		}
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
