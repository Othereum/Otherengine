#include "actors/ship.h"
#include "actors/laser.h"
#include <array>
#include "components/sprite_component.h"
#include "components/input_component.h"
#include "components/pawn_move_comp.h"
#include "application.h"

namespace game
{
	static constexpr std::array ship_png{"Assets/Ship.png", "Assets/ShipWithThrust.png"};
	
	struct input_forward : input_axis
	{
		[[nodiscard]] std::vector<axis_t> keys() const override
		{
			return {{'w', 1}, {'s', -1}};
		}
	};
	
	struct input_rotate : input_axis
	{
		[[nodiscard]] std::vector<axis_t> keys() const override
		{
			return {{'a', -1}, {'d', 1}};
		}
	};

	struct input_shoot : input_action
	{
		[[nodiscard]] std::unordered_set<key_t> keys() const override
		{
			return {' '};
		}
	};

	ship::ship(application& app)
		:actor{app}
	{
		auto& sprite = add_component<sprite_component>();
		sprite.set_texture(ship_png[0]);

		auto& movement = add_component<pawn_move_comp>();

		auto& input = add_component<input_component>();
		input.bind_axis(input_forward{}, [&](float f)
		{
			const auto should_move = !math::is_nearly_zero(f);
			if (should_move) movement.add_input(get_forward() * f);

			if (should_move != is_moving_)
			{
				sprite.set_texture(ship_png[should_move]);
				is_moving_ = should_move;
			}
		});

		input.bind_axis(input_rotate{}, [&](float f)
		{
			movement.add_rotation_input(f);
		});

		input.bind_action(input_shoot{}, key_event::pressed, [this]()
		{
			auto& l = get_app().spawn_actor<laser>();
			l.set_pos(get_pos());
			l.set_rot(get_rot());
		});
	}
}
