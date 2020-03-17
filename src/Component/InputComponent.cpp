#include <SDL_keyboard.h>
#include "Component/InputComponent.h"
#include "Engine.h"

namespace Game
{
	CInputComponent::CInputComponent(AActor& owner, int updateOrder, int inputReceiveOrder)
		:CActorComponent{owner, updateOrder}, receiveOrder_{inputReceiveOrder}
	{
	}

	CInputComponent::~CInputComponent()
	{
		GetEngine().UnregisterInputComponent(*this);
	}

	void CInputComponent::BeginPlay()
	{
		GetEngine().RegisterInputComponent(*this);
	}

	void CInputComponent::ProcessInput(const std::vector<int> (&events)[2], const uint8_t* keyboard) const
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

	void CInputComponent::BindAction(const FInputAction& action, EKeyEvent event, std::function<void()>&& callback)
	{
		actions_[int(event)].emplace_back(action.Keys(), std::move(callback));
	}

	void CInputComponent::BindAxis(const FInputAxis& axis, std::function<void(float)>&& callback)
	{
		axises_.emplace_back(axis.Keys(), std::move(callback));
	}
}
