#pragma once
#include <memory>
#include <vector>

struct SDL_Texture;

namespace game
{
	class actor;
	class sprite_component;
	class application;
	
	class world
	{
	public:
		explicit world(application& app): app_{app} {}

		template <class T>
		T& spawn_actor()
		{
			static_assert(std::is_base_of_v<actor, T>);
			auto ptr = std::make_unique<T>(*this);
			auto& actor = *ptr;
			register_actor(std::move(ptr));
			return actor;
		}
		
		void update(float delta_seconds);

		[[nodiscard]] application& get_app() const { return app_; }

	private:
		void register_actor(std::unique_ptr<actor>&& actor);

		bool is_updating_actors_ = false;
		application& app_;
		std::vector<std::unique_ptr<actor>> actors_;
		std::vector<std::unique_ptr<actor>> pending_actors_;
	};
}
