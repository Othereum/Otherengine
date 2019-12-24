#include "Game.h"
#include "SDL.h"
#include "Exception.h"
#include "StringUtils.h"

NEG_BEGIN

SDL_Window* create_window()
{	
	const auto window = SDL_CreateWindow("No Engine Game", 100, 100, 1024, 768, 0);
	if (!window) throw sdl_error{fmt("Failed to create window: ", SDL_GetError())};
	return window;
}

SDL_Renderer* create_renderer(SDL_Window* const window)
{
	const auto renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!renderer) throw sdl_error{fmt("Failed to create renderer: ", SDL_GetError())};
	return renderer;
}

game::game()
	:window_{create_window(), SDL_DestroyWindow},
	renderer_{create_renderer(window_.get()), SDL_DestroyRenderer},
	is_running_{true}
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
}

void game::generate_output()
{
	SDL_SetRenderDrawColor(renderer_.get(), 0, 0, 255, 255);
	SDL_RenderClear(renderer_.get());
	SDL_RenderPresent(renderer_.get());
}

game::sdl_raii::sdl_raii()
{
	const auto sdl_result = SDL_Init(SDL_INIT_VIDEO);
	if (sdl_result != 0) throw sdl_error{fmt("Unable to initialize SDL: ", SDL_GetError())};
}

game::sdl_raii::~sdl_raii()
{
	SDL_Quit();
}

NEG_END
