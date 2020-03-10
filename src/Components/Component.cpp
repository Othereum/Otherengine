#include "components/component.h"
#include "actors/actor.h"

namespace game
{
	component::component(actor& owner, const int update_order)
		:owner_{owner}, update_order_{update_order}
	{
	}

	world& component::get_world() const noexcept
	{
		return get_owner().get_world();
	}

	application& component::get_app() const noexcept
	{
		return get_owner().get_app();
	}
}
