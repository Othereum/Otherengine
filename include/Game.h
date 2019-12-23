#pragma once

#include "NEG.h"
#include <memory>
#include <unordered_map>
#include <functional>

class SDL_Window;

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
	void generate_input();

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
	unsigned is_running_ : 1;
};

NEG_END
