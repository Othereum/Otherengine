#include "Game.h"
#include "SDL.h"
#include "Exception.h"
#include "StringUtils.h"

NEG_BEGIN

game::game() noexcept
	:window_{nullptr, SDL_DestroyWindow}, is_running_{true}
{
}

game::~game() = default;

void game::initialize()
{
	const auto sdl_result = SDL_Init(SDL_INIT_VIDEO);
	if (sdl_result != 0) throw sdl_error{fmt("Unable to initialize SDL: ", SDL_GetError())};
}

NEG_END
