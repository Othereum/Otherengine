#include "Exception.h"
#include "SDL.h"

NEG_BEGIN

sdl_error::sdl_error(const char* const what_arg)
	:runtime_error{what_arg}
{
	SDL_Log("%s", what_arg);
}

sdl_error::sdl_error(const std::string& what_arg)
	:runtime_error{what_arg}
{
	SDL_Log("%s", what_arg.c_str());
}

NEG_END
