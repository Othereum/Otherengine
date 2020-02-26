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
		:actor{outer}
	{
		auto& renderer = outer.get_app().get_renderer();
		auto& anim = add_component<anim_sprite_component>();
		anim.set_anim_textures({
			renderer.get_texture("Assets/Ship01.png"),
			renderer.get_texture("Assets/Ship02.png"),
			renderer.get_texture("Assets/Ship03.png"),
			renderer.get_texture("Assets/Ship04.png")
		});
	}

	void ship::update_actor(const float delta_seconds)
	{
		set_pos(get_pos() + velocity_ * delta_seconds);
		velocity_.x = math::clamp(velocity_.x, 25, 500);
		velocity_.y = math::clamp(velocity_.y, 25, 743);
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
	}
}
