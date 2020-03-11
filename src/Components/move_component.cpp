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
		owner.set_pos(owner.get_pos() + vel_ * delta_seconds);
		owner.set_rot(owner.get_rot() + rvel_ * delta_seconds);
	}
}
