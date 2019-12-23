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

game::game()
	: window_{create_window(), SDL_DestroyWindow}, is_running_{true}, is_shutdown_{false}
{
}

game::~game() = default;

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
