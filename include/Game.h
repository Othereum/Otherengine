#pragma once

#include "NEG.h"
#include <memory>

class SDL_Window;

NEG_BEGIN

class game
{
public:
	game();
	~game();
	void run_loop();

private:
	void process_input();
	void update_game();
	void generate_input();

	struct sdl_raii
	{
		sdl_raii();
		~sdl_raii();
	} sdl_raii_;
	
	std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window_;
	uint8_t is_running_ : 1;
	uint8_t is_shutdown_ : 1;
};

NEG_END
