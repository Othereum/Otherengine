#include "game.h"
#include <thread>
#include <stdexcept>
#include <SDL.h>
#include <SDL_image.h>
#include "math_utils.h"
#include "actors/actor.h"
#include "components/sprite_component.h"

NEG_BEGIN

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

game::game():
	is_running_{true}, is_updating_actors_{},
	window_{create_window(), SDL_DestroyWindow},
	renderer_{create_renderer(window_.get()), SDL_DestroyRenderer}
{
	refresh_rate_ = get_refresh_rate(window_.get());
	load_data();
}

game::~game() = default;

void game::run_loop()
{
	while (is_running_)
	{
		process_input();
		update_game();
		generate_output();
	}
}

void game::shutdown()
{
	is_running_ = false;
}

void game::add_actor(actor_ptr&& actor)
{
	if (is_updating_actors_)
	{
		pending_actors_.push_back(std::move(actor));
	}
	else
	{
		actors_.push_back(std::move(actor));
	}
}

std::shared_ptr<SDL_Texture> game::load_texture(const char* const filename)
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

std::shared_ptr<SDL_Texture> game::get_texture(const char* const filename)
{
	const auto found = textures_.find(filename);
	if (found != textures_.end()) return found->second.lock();

	const auto loaded = load_texture(filename);
	textures_.emplace(filename, loaded);

	return loaded;
}

void game::add_sprite(const sprite_component& sprite)
{
	auto cmp = [](const sprite_component& a, const sprite_component& b)
	{
		return a.get_draw_order() <= b.get_draw_order();
	};
	const auto pos = std::lower_bound(sprites_.begin(), sprites_.end(), sprite, cmp);
	sprites_.emplace(pos, sprite);
}

void game::remove_sprite(const sprite_component& sprite)
{
	auto pr = [&](const sprite_component& v) { return &v == &sprite; };
	const auto found = std::find_if(sprites_.crbegin(), sprites_.crend(), pr);
	if (found != sprites_.crend()) sprites_.erase(found.base() - 1);
}

void game::load_data()
{
	class othereum : public actor
	{
	public:
		explicit othereum(neg::game& g): actor{g} {}
		void update_actor(const float s) override { set_rot(get_rot() + s * 360_deg); }
	};
	
	auto& a = spawn_actor<othereum>();
	a.set_pos({screen_w / 2.f, screen_h / 2.f});

	auto& c = a.add_component<sprite_component>();
	c.set_texture(get_texture("Othereum.png"));
}

void game::process_input()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			shutdown();
			break;
		default:
			break;
		}
	}

	const auto keyboard = SDL_GetKeyboardState(nullptr);
	if (keyboard[SDL_SCANCODE_ESCAPE]) shutdown();
}

void game::update_game()
{
	using namespace std::chrono;

	constexpr nanoseconds sec = 1s;
	constexpr auto min_fps = 10;
	constexpr auto time_speed = 1;

	std::this_thread::sleep_until(time_ + sec / refresh_rate_);

	const auto now = steady_clock::now();
	const auto delta_seconds = duration<float>{math::min(now - time_, sec / min_fps) * time_speed}.count();
	time_ = now;

	is_updating_actors_ = true;
	for (const auto& actor : actors_)
	{
		actor->update(delta_seconds);
	}
	is_updating_actors_ = false;

	for (auto&& pending : pending_actors_)
	{
		actors_.push_back(std::move(pending));
	}
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

void game::generate_output() const
{
	SDL_SetRenderDrawColor(renderer_.get(), 100, 100, 250, 255);
	SDL_RenderClear(renderer_.get());

	for (auto& sprite : sprites_)
	{
		sprite.get().draw(renderer_.get());
	}

	SDL_RenderPresent(renderer_.get());
}

game::sdl_raii::sdl_raii()
{
	const auto sdl_result = SDL_Init(SDL_INIT_VIDEO);
	if (sdl_result != 0) throw std::runtime_error{SDL_GetError()};

	const auto flags = IMG_INIT_PNG;
	const auto img_result = IMG_Init(flags);
	if (img_result != flags) throw std::runtime_error{IMG_GetError()};
}

game::sdl_raii::~sdl_raii()
{
	IMG_Quit();
	SDL_Quit();
}

NEG_END
