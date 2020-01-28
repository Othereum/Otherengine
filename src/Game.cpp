#include <Game.h>
#include <thread>
#include <SDL.h>
#include <SDL_image.h>
#include <Exception.h>
#include <StringUtils.h>
#include <MathUtils.h>
#include <Actor.h>

NEG_BEGIN

static constexpr auto screen_w = 1024;
static constexpr auto screen_h = 768;

static SDL_Window* create_window()
{	
	const auto window = SDL_CreateWindow("No Engine Game", 100, 100, screen_w, screen_h, 0);
	if (!window) throw sdl_error{join("Failed to create window: ", SDL_GetError())};
	return window;
}

static SDL_Renderer* create_renderer(SDL_Window* const window)
{
	const auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) throw sdl_error{join("Failed to create renderer: ", SDL_GetError())};
	return renderer;
}

game::game()
	:is_running_{true}, is_updating_actors_{},
	window_{create_window(), SDL_DestroyWindow},
	renderer_{create_renderer(window_.get()), SDL_DestroyRenderer}
{
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
	constexpr auto max_fps = 60, min_fps = 10;
	constexpr auto time_scale = 1.f;
	
	std::this_thread::sleep_for(std::chrono::milliseconds{ticks_count_ + 1000ll/max_fps - SDL_GetTicks()});
	
	const auto delta_time = math::min((SDL_GetTicks() - ticks_count_) / 1000.f, 1.f/min_fps) * time_scale;
	ticks_count_ = SDL_GetTicks();

	is_updating_actors_ = true;
	for (const auto& actor : actors_)
	{
		actor->update(delta_time);
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
	
	SDL_RenderPresent(renderer_.get());
}

game::sdl_raii::sdl_raii()
{
	const auto sdl_result = SDL_Init(SDL_INIT_VIDEO);
	if (sdl_result != 0) throw sdl_error{join("Unable to initialize SDL: ", SDL_GetError())};

	const auto flags = IMG_INIT_PNG;
	const auto img_result = IMG_Init(flags);
	if (img_result != flags) throw sdl_error{join("Unable to initialize SDL image: ", IMG_GetError())};
}

game::sdl_raii::~sdl_raii()
{
	IMG_Quit();
	SDL_Quit();
}

NEG_END
