#include "application.h"
#include <thread>
#include <stdexcept>
#include <SDL.h>
#include <SDL_image.h>
#include "actors/actor.h"
#include "components/sprite_component.h"
#include "math_utils.h"
#include "world.h"

namespace game
{
	static constexpr auto screen_w = 1024;
	static constexpr auto screen_h = 768;

	static SDL_Window* create_window()
	{
		const auto window = SDL_CreateWindow(PROJECT_NAME, 100, 100, screen_w, screen_h, 0);
		if (!window) throw std::runtime_error{SDL_GetError()};
		return window;
	}

	static SDL_Renderer* create_renderer(SDL_Window* const window)
	{
		const auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		if (!renderer) throw std::runtime_error{SDL_GetError()};
		return renderer;
	}

	static uint8_t get_refresh_rate(SDL_Window* const window)
	{
		SDL_DisplayMode mode;
		if (SDL_GetWindowDisplayMode(window, &mode) != 0) throw std::runtime_error{SDL_GetError()};
		return mode.refresh_rate;
	}

	application::application():
		window_{create_window(), SDL_DestroyWindow},
		renderer_{create_renderer(window_.get()), SDL_DestroyRenderer},
		world_{std::make_unique<world>(*this)}
	{
		refresh_rate_ = get_refresh_rate(window_.get());
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

	void application::add_sprite(const sprite_component& sprite)
	{
		auto cmp = [](const sprite_component& a, const sprite_component& b)
		{
			return a.get_draw_order() <= b.get_draw_order();
		};
		const auto pos = std::lower_bound(sprites_.begin(), sprites_.end(), sprite, cmp);
		sprites_.emplace(pos, sprite);
	}

	void application::remove_sprite(const sprite_component& sprite)
	{
		auto pr = [&](const sprite_component& v) { return &v == &sprite; };
		const auto found = std::find_if(sprites_.crbegin(), sprites_.crend(), pr);
		if (found != sprites_.crend()) sprites_.erase(found.base() - 1);
	}
	
	std::shared_ptr<SDL_Texture> application::load_texture(const char* const filename)
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

	std::shared_ptr<SDL_Texture> application::get_texture(const char* const filename)
	{
		const auto found = textures_.find(filename);
		if (found != textures_.end()) return found->second.lock();

		const auto loaded = load_texture(filename);
		textures_.emplace(filename, loaded);

		return loaded;
	}

	void application::load_data()
	{
		class othereum : public actor
		{
		public:
			explicit othereum(world& outer): actor{outer} {}
			void update_actor(const float s) override { set_rot(get_rot() + s * 360_deg); }
		};
		
		auto& a = world_->spawn_actor<othereum>();
		a.set_pos({screen_w / 2.f, screen_h / 2.f});

		auto& c = a.add_component<sprite_component>();
		c.set_texture(get_texture("Othereum.png"));
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

	void application::generate_output() const
	{
		SDL_SetRenderDrawColor(renderer_.get(), 100, 100, 250, 255);
		SDL_RenderClear(renderer_.get());

		for (auto& sprite : sprites_)
		{
			sprite.get().draw(renderer_.get());
		}

		SDL_RenderPresent(renderer_.get());
	}

	application::sdl_raii::sdl_raii()
	{
		const auto sdl_result = SDL_Init(SDL_INIT_VIDEO);
		if (sdl_result != 0) throw std::runtime_error{SDL_GetError()};

		const auto flags = IMG_INIT_PNG;
		const auto img_result = IMG_Init(flags);
		if (img_result != flags) throw std::runtime_error{IMG_GetError()};
	}

	application::sdl_raii::~sdl_raii()
	{
		IMG_Quit();
		SDL_Quit();
	}
}