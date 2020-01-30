#include "actors/actor.h"
#include "components/component.h"

namespace game
{
	actor::actor(world& outer)
		:world_{outer}
	{
	}

	void actor::update(const float delta_seconds)
	{
		update_components(delta_seconds);
		update_actor(delta_seconds);
	}

	void actor::update_components(const float delta_seconds)
	{
		for (const auto& comp : comps_)
		{
			comp->update(delta_seconds);
		}
	}

	void actor::register_component(std::unique_ptr<component>&& comp)
	{
		auto cmp = [](const std::unique_ptr<component>& a, const std::unique_ptr<component>& b)
		{
			return a->get_update_order() <= b->get_update_order();
		};
		const auto pos = std::lower_bound(comps_.begin(), comps_.end(), comp, cmp);
		comps_.insert(pos, std::move(comp));
	}

	void actor::destroy()
	{
		state_ = state::dead;
	}
}
