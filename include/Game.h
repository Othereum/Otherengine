#pragma once

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

	class SDL_Window* window_;
	bool is_running_;
};
