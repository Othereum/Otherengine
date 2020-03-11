#include "components/move_component.h"
#include "actors/actor.h"

namespace game
{
	move_component::move_component(actor& owner, int update_order)
		:component{owner, update_order}
	{
	}

	void move_component::update(float delta_seconds)
	{
		auto& owner = get_owner();
		owner.set_pos(owner.get_pos() + movement_);
		owner.set_rot(owner.get_rot() + rotation_);

		movement_ = {};
		rotation_ = {};
	}

	void move_component::add_movement_input(fvector2 v)
	{
		movement_ += v;
	}

	void move_component::add_rotation_input(degrees r)
	{
		rotation_ += r;
	}
}
