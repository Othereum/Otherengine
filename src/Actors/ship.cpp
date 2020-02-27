#include "actors/ship.h"
#include <SDL.h>
#include "components/anim_sprite_component.h"
#include "world.h"
#include "application.h"
#include "renderer.h"
#include "math_utils.h"

namespace game
{
	ship::ship(world& outer)
		:actor{outer}, anim_{add_component<anim_sprite_component>()}
	{
		auto& renderer = outer.get_app().get_renderer();
		anim_.add_anim({{
			renderer.get_texture("Assets/Character01.png"),
			renderer.get_texture("Assets/Character02.png"),
			renderer.get_texture("Assets/Character03.png"),
			renderer.get_texture("Assets/Character04.png"),
			renderer.get_texture("Assets/Character05.png"),
			renderer.get_texture("Assets/Character06.png"),
		}, 8, true});
		anim_.add_anim({{
			renderer.get_texture("Assets/Character16.png"),
			renderer.get_texture("Assets/Character17.png"),
			renderer.get_texture("Assets/Character18.png"),
		}, 8, false});
	}

	void ship::update_actor(const float delta_seconds)
	{
		auto newpos = get_pos() + velocity_ * delta_seconds;
		newpos.x = math::clamp(newpos.x, 25, 500);
		newpos.y = math::clamp(newpos.y, 25, 743);
		set_pos(newpos);
	}

	void ship::process_keyboard(const uint8_t* state)
	{
		velocity_ = {};
		
		if (state[SDL_SCANCODE_W])
			velocity_.y -= 300;

		if (state[SDL_SCANCODE_S])
			velocity_.y += 300;

		if (state[SDL_SCANCODE_A])
			velocity_.x -= 250;

		if (state[SDL_SCANCODE_D])
			velocity_.x += 250;

		if (state[SDL_SCANCODE_Q])
			anim_.play(0);

		if (state[SDL_SCANCODE_E])
			anim_.play(1);
	}
}
