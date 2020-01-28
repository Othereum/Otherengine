#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <NEG.h>

class SDL_Window;
class SDL_Renderer;
class SDL_Texture;

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

	template <class T>
	T& spawn_actor()
	{
		static_assert(std::is_base_of_v<actor, T>);
		auto ptr = std::make_unique<T>(*this);
		auto& actor = *ptr;
		add_actor(std::move(ptr));
		return actor;
	}

	std::shared_ptr<SDL_Texture> get_texture(const char* filename);
	
	void add_sprite(const class sprite_component& sprite);
	void remove_sprite(const sprite_component& sprite);

private:
	void process_input();
	void update_game();
	void generate_output() const;

	void add_actor(actor_ptr&& actor);

	std::shared_ptr<SDL_Texture> load_texture(const char* filename);

	struct sdl_raii
	{
		sdl_raii();
		~sdl_raii();
		sdl_raii(const sdl_raii&) = delete;
		sdl_raii(sdl_raii&&) = delete;
		sdl_raii& operator=(const sdl_raii&) = delete;
		sdl_raii& operator=(sdl_raii&&) = delete;
	} sdl_raii_;

	bool is_running_ : 1;
	bool is_updating_actors_ : 1;
	
	unsigned ticks_count_{};
	
	std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> window_;
	std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> renderer_;

	std::vector<actor_ptr> actors_;
	std::vector<actor_ptr> pending_actors_;
	std::vector<std::reference_wrapper<const sprite_component>> sprites_;
	
	std::unordered_map<std::string, std::weak_ptr<SDL_Texture>> textures_;
};

NEG_END
