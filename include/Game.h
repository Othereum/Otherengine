#pragma once

#include <memory>
#include "NEG.h"
#include "Vector.h"

class SDL_Window;
class SDL_Renderer;

NEG_BEGIN

class game
{
public:
	game();
	~game();
	game(const game&) = delete;
	game(game&&) = delete;
	game& operator=(const game&) = delete;
	game& operator=(game&&) = delete;
	
	void run_loop();
	void shutdown();

private:
	void process_input();
	void update_game();
	void generate_output();

	struct sdl_raii
	{
		sdl_raii();
		~sdl_raii();
		sdl_raii(const sdl_raii&) = delete;
		sdl_raii(sdl_raii&&) = delete;
		sdl_raii& operator=(const sdl_raii&) = delete;
		sdl_raii& operator=(sdl_raii&&) = delete;
	} sdl_raii_;

	std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window_;
	std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> renderer_;

	vector2 paddle_pos_, ball_pos_;

	unsigned ticks_count_;
	
	unsigned is_running_ : 1;
};

NEG_END
