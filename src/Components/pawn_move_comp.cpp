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
		if (lensqr > 1) input_ /= sqrtf(lensqr);

		rot_input_ = math::clamp(rot_input_, -1, 1);

		set_velocity(input_ * max_speed_);
		set_angular_velocity(rot_input_ * max_rot_speed_);

		input_ = {};
		rot_input_ = {};

		move_component::update(delta_seconds);
	}
}
