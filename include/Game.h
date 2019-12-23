#pragma once

#include <memory>
#include "NEG.h"

class SDL_Window;

NEG_BEGIN

class game
{
public:
	game() noexcept;
	~game();
	void initialize();
	void run_loop();
	void shutdown();

private:
	void process_input();
	void update_game();
	void generate_input();

	std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window_;
	bool is_running_;
};

NEG_END
