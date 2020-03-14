#include "actors/asteroid.h"
#include "components/move_component.h"
#include "components/sprite_component.h"
#include "math_utils.h"

namespace game
{
	asteroid::asteroid(application& app)
		:actor{app}
	{
		auto& sprite = add_component<sprite_component>();
		sprite.set_texture("Assets/Asteroid.png");

		auto& movement = add_component<move_component>();
		movement.set_velocity(math::rand_unit_vec() * 150);
	}
}
