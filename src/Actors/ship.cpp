#include "actors/ship.h"
#include "components/sprite_component.h"
#include "components/input_component.h"
#include "components/pawn_move_comp.h"

namespace game
{
	ship::ship(application& app)
		:actor{app}
	{
		auto& sprite = add_component<sprite_component>();
		sprite.set_texture("Assets/Ship.png");

		auto& movement = add_component<pawn_move_comp>();

		struct input_forward : input_axis
		{
			[[nodiscard]] std::vector<axis_t> keys() const noexcept override
			{
				return {{'w', 1}, {'s', -1}};
			}
		};
		
		struct input_rotate : input_axis
		{
			[[nodiscard]] std::vector<axis_t> keys() const noexcept override
			{
				return {{'a', -1}, {'d', 1}};
			}
		};

		auto& input = add_component<input_component>();
		input.bind_axis(input_forward{}, [&](float f)
		{
			if (!math::is_nearly_zero(f))
			{
				movement.add_input(get_forward() * f);
				if (!is_moving_)
				{
					sprite.set_texture("Assets/ShipWithThrust.png");
					is_moving_ = true;
				}
			}
			else if (is_moving_)
			{
				sprite.set_texture("Assets/Ship.png");
				is_moving_ = false;
			}
		});

		input.bind_axis(input_rotate{}, [&](float f)
		{
			movement.add_rotation_input(f);
		});
	}
}
