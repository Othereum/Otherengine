#pragma once

#include <memory>
#include "NEG.h"
#include <vector>

class SDL_Window;
class SDL_Renderer;

NEG_BEGIN

class game
{
public:
	using actor_ptr = std::unique_ptr<class actor>;
	
	game();
	~game();
	game(const game&) = delete;
	game(game&&) = delete;
	game& operator=(const game&) = delete;
	game& operator=(game&&) = delete;
	
	void run_loop();
	void shutdown();

	void add_actor(actor_ptr&& actor);
	void remove_actor(const actor& actor);

private:
	void process_input();
	void update_game();
	void generate_output() const;

	struct sdl_raii
	{
		sdl_raii();
		~sdl_raii();
		sdl_raii(const sdl_raii&) = delete;
		sdl_raii(sdl_raii&&) = delete;
		sdl_raii& operator=(const sdl_raii&) = delete;
		sdl_raii& operator=(sdl_raii&&) = delete;
	} sdl_raii_;

	bool is_running_;
	unsigned ticks_count_;
	
	std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window_;
	std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> renderer_;

	std::vector<actor_ptr> actors_;
	std::vector<actor_ptr> pending_actors_;
};

NEG_END
