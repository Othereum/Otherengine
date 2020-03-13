#include "components/circle_component.h"
#include "actors/actor.h"

namespace game
{
	circle_component::circle_component(actor& owner, int update_order)
		:component{owner, update_order}
	{
	}

	void circle_component::test_overlap(circle_component& other) const
	{
		if (!is_bound_on_overlap()) return;

		const auto distsqr = get_owner().get_pos().distsqr(other.get_owner().get_pos());
		const auto r = radius_ + other.radius_;
		
		if (distsqr < r*r) on_overlap_(other);
	}

	void circle_component::bind_on_overlap(std::function<void(circle_component&)>&& on_overlap) noexcept
	{
		on_overlap_ = std::move(on_overlap);
	}
}
