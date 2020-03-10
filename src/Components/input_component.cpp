#include "components/input_component.h"

namespace game
{
	input_component::input_component(actor& owner, int update_order)
		:component{owner, update_order}
	{
	}

	void input_component::bind_action(const input_action& action, key_event event, std::function<void()>&& callback)
	{
		const auto idx = action_fns_.size();
		action_fns_.emplace_back(std::move(callback));
		
		const auto [keys, size] = action.keys();
		for (size_t i = 0; i < size; ++i)
		{
			actions_[int(event)].emplace(keys[i], idx);
		}
	}

	void input_component::bind_axis(const input_axis& axis, std::function<void(float)>&& callback)
	{
		const auto idx = axis_fns_.size();
		axis_fns_.emplace_back(std::move(callback));
		
		const auto [axises, size] = axis.keys();
		for (size_t i = 0; i < size; ++i)
		{
			axises_.emplace_back(axises[i], idx);
		}
	}
}
