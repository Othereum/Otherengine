#include "world.h"
#include "actors/actor.h"

namespace game
{
	void world::update(const float delta_seconds)
	{
		is_updating_actors_ = true;
		for (const auto& actor : actors_)
		{
			actor->update(delta_seconds);
		}
		is_updating_actors_ = false;

		for (auto&& pending : pending_actors_)
		{
			actors_.push_back(std::move(pending));
		}
		pending_actors_.clear();

		for (auto it = actors_.rbegin(); it != actors_.rend();)
		{
			const auto& actor = **it;
			if (actor.get_state() == actor::state::dead)
			{
				const auto next = actors_.erase(it.base() - 1);
				it = std::make_reverse_iterator(next);
			}
			else
			{
				++it;
			}
		}
	}

	void world::register_actor(std::unique_ptr<actor>&& actor)
	{
		(is_updating_actors_ ? pending_actors_ : actors_).push_back(std::move(actor));
	}
}
