#include "components/input_component.h"
#include <SDL_keyboard.h>
#include "application.h"

namespace game
{
	input_component::input_component(actor& owner, int update_order, int input_receive_order)
		:component{owner, update_order}, receive_order_{input_receive_order}
	{
		get_app().register_input_component(*this);
	}

	input_component::~input_component()
	{
		get_app().unregister_input_component(*this);
	}

	void input_component::process_input(const std::vector<int> (&events)[2], const uint8_t* keyboard) const
	{
		for (auto i = 0; i < 2; ++i) for (auto& action : actions_[i]) for (auto key : events[i])
		{
			if (action.first.contains(key))
			{
				action.second();
				break;
			}
		}
		
		for (auto& axis : axises_)
		{
			auto val = 0.f;
			
			for (auto& key : axis.first)
				if (keyboard[SDL_GetScancodeFromKey(key.key)])
					val += key.scale;
			
			axis.second(val);
		}
	}

	void input_component::bind_action(const input_action& action, key_event event, std::function<void()>&& callback)
	{
		actions_[int(event)].emplace_back(action.keys(), std::move(callback));
	}

	void input_component::bind_axis(const input_axis& axis, std::function<void(float)>&& callback)
	{
		axises_.emplace_back(axis.keys(), std::move(callback));
	}
}
