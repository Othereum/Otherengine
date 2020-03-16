#include "application.h"

#include <stdexcept>
#include <SDL.h>
#include <SDL_image.h>

#include "actors/actor.h"
#include "components/input_component.h"
#include "components/sprite_component.h"
#include "components/circle_component.h"

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

	static renderer_ptr create_renderer(SDL_Window& window)
	{
		renderer_ptr renderer{
			SDL_CreateRenderer(&window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC),
			SDL_DestroyRenderer
		};
		if (!renderer) throw std::runtime_error{SDL_GetError()};
		return renderer;
	}
	
	application::application() :
		window_{create_window()},
		renderer_{create_renderer(*window_)},
		time_{std::chrono::steady_clock::now()}
	{
		for (auto i = 0; i < 20; ++i)
		{
			auto& ast = spawn_actor<asteroid>();
			ast.set_pos(math::rand_vec({0, 0}, fvector2{scrsz}));
			ast.set_rot(math::rand_ang());
		}

		auto& sh = spawn_actor<ship>();
		sh.set_pos(fvector2{scrsz / 2});
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

	void application::register_collision(circle_component& comp)
	{
		collisions_.emplace_back(comp);
	}

	void application::unregister_collision(circle_component& comp)
	{
		auto pr = [&](const circle_component& v) { return &v == &comp; };
		const auto found = std::find_if(collisions_.crbegin(), collisions_.crend(), pr);
		if (found != collisions_.crend()) collisions_.erase(found.base() - 1);
	}

	void application::register_sprite(const sprite_component& sprite)
	{
		auto cmp = [](const sprite_component& a, const sprite_component& b)
		{
			return a.get_draw_order() <= b.get_draw_order();
		};
		const auto pos = std::lower_bound(sprites_.begin(), sprites_.end(), sprite, cmp);
		sprites_.emplace(pos, sprite);
	}

	void application::unregister_sprite(const sprite_component& sprite)
	{
		auto pr = [&](const sprite_component& v) { return &v == &sprite; };
		const auto found = std::find_if(sprites_.crbegin(), sprites_.crend(), pr);
		if (found != sprites_.crend()) sprites_.erase(found.base() - 1);
	}

	void application::draw(SDL_Texture& texture, const frect& dest, degrees angle) const
	{
		const SDL_Rect r = dest;
		SDL_RenderCopyEx(renderer_.get(), &texture, nullptr, &r, angle.get(), nullptr, SDL_FLIP_NONE);
	}

	void application::draw(SDL_Texture& texture, const SDL_Rect& src, const SDL_Rect& dest, degrees angle) const
	{
		SDL_RenderCopyEx(renderer_.get(), &texture, &src, &dest, angle.get(), nullptr, SDL_FLIP_NONE);
	}

	std::shared_ptr<SDL_Texture> application::get_texture(const char* filename)
	{
		const auto found = textures_.find(filename);
		if (found != textures_.end()) return found->second.lock();

		const auto loaded = load_texture(filename);
		textures_.emplace(filename, loaded);

		return loaded;
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
				if (!event.key.repeat) key_events[event.key.state].push_back(event.key.keysym.sym);
				break;
				
			default: ;
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
		for (const auto& actor : actors_)
			actor->update(delta_seconds);

		
		for (size_t i = 0; i < collisions_.size(); ++i)
			for (auto j = i+1; j < collisions_.size(); ++j)
				collisions_[i].get().test_overlap(collisions_[j]);

		
		for (auto&& pending : pending_actors_)
			actors_.push_back(std::move(pending));
		
		pending_actors_.clear();

		
		for (auto it = actors_.rbegin(); it != actors_.rend();)
		{
			const auto& actor = **it;
			if (actor.get_state() == actor::state::dead)
			{
				const auto next = actors_.erase(it.base() - 1);
				it = std::make_reverse_iterator(next);
			}
			else
			{
				++it;
			}
		}
	}

	void application::generate_output()
	{
		SDL_RenderClear(renderer_.get());

		for (auto& sprite : sprites_)
		{
			sprite.get().draw();
		}

		SDL_RenderPresent(renderer_.get());
	}

	float application::update_time()
	{
		using namespace std::chrono;
		const auto now = steady_clock::now();
		const auto delta_seconds = duration<float>{now - time_}.count();
		time_ = now;
		return delta_seconds;
	}

	void application::register_actor(std::unique_ptr<actor>&& actor)
	{
		pending_actors_.push_back(std::move(actor));
	}

	std::shared_ptr<SDL_Texture> application::load_texture(const char* filename)
	{
		const std::unique_ptr<SDL_Surface, void(*)(SDL_Surface*)> surface{IMG_Load(filename), SDL_FreeSurface};
		if (!surface) throw std::runtime_error{SDL_GetError()};

		auto deleter = [this, filename](SDL_Texture* const texture)
		{
			textures_.erase(filename);
			SDL_DestroyTexture(texture);
		};
		std::shared_ptr<SDL_Texture> texture{SDL_CreateTextureFromSurface(renderer_.get(), surface.get()), std::move(deleter)};
		if (!texture) throw std::runtime_error{SDL_GetError()};

		return texture;
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
