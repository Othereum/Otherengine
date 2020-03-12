#include "components/pawn_move_comp.h"

namespace game
{
	pawn_move_comp::pawn_move_comp(actor& owner, int update_order)
		:move_component{owner, update_order}
	{
	}

	void pawn_move_comp::update(float delta_seconds)
	{
		const auto lensqr = input_.lensqr();
		if (lensqr > max_speed_*max_speed_)
		{
			input_ *= max_speed_ / sqrtf(lensqr);
		}

		rot_input_ = math::clamp(rot_input_, -max_rot_speed_, max_rot_speed_);

		set_velocity(input_);
		set_angular_velocity(rot_input_);

		input_ = {};
		rot_input_ = {};

		move_component::update(delta_seconds);
	}
}
