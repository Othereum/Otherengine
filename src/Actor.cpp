#include "Actor.h"
#include "Component.h"

NEG_BEGIN

actor::actor(game& game)
	:game_{game}
{
}

void actor::update(const float delta_time)
{
	update_components(delta_time);
	update_actor(delta_time);
}

void actor::update_components(const float delta_time)
{
	for (const auto& comp : comps_)
		comp->update(delta_time);
}

void actor::add_component(comp_ptr&& comp)
{
	auto cmp = [](const comp_ptr& a, const comp_ptr& b)
	{
		return a->get_update_order() <= b->get_update_order();
	};
	const auto pos = std::lower_bound(comps_.begin(), comps_.end(), comp, cmp);
	comps_.insert(pos, std::move(comp));
}

void actor::remove_component(const component& comp)
{
	auto p = [&](const comp_ptr& v) { return v.get() == &comp; };
	const auto found = std::find_if(comps_.begin(), comps_.end(), p);
	if (found != comps_.end()) comps_.erase(found);
}

NEG_END
