#include "components/circle_component.h"
#include "actors/actor.h"
#include "application.h"

namespace game
{
	circle_component::circle_component(actor& owner, int update_order)
		:component{owner, update_order}
	{
	}

	circle_component::~circle_component()
	{
		get_app().unregister_collision(*this);
	}

	void circle_component::begin_play()
	{
		get_app().register_collision(*this);
	}

	void circle_component::test_overlap(circle_component& other)
	{
		if (!on_overlap_ && !other.on_overlap_) return;
		
		const auto distsqr = get_owner().get_pos().distsqr(other.get_owner().get_pos());
		const auto r = radius_ + other.radius_;

		if (distsqr < r*r)
		{
			if (on_overlap_) on_overlap_(other);
			if (other.on_overlap_) other.on_overlap_(*this);
		}
	}

	void circle_component::bind_on_overlap(std::function<void(circle_component&)>&& on_overlap) noexcept
	{
		on_overlap_ = std::move(on_overlap);
	}
}
