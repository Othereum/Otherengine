#pragma once

#include <memory>

class game
{
public:
	game();
	bool initialize();
	void run_loop();
	void shutdown();

private:
	void process_input();
	void update_game();
	void generate_input();

	std::unique_ptr<class SDL_Window, void(SDL_Window*)> window_;
	bool is_running_;
};
