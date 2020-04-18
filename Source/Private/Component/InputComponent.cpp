#include "Component/InputComponent.h"
#include "Engine.h"
#include "InputSystem.h"

namespace oeng
{
	CInputComponent::CInputComponent(AActor& owner, int updateOrder)
		:CActorComponent{owner, updateOrder}
	{
	}

	void CInputComponent::Update(float deltaSeconds)
	{
		if (IsEnabled())
		{
			ProcessActions();
			ProcessAxises();
		}
	}

	void CInputComponent::BindAction(FName action, bool bPressed, std::function<void()>&& callback)
	{
		actions_[bPressed].emplace(action, std::move(callback));
	}

	void CInputComponent::BindAxis(FName axis, std::function<void(float)>&& callback)
	{
		axises_.emplace(axis, std::move(callback));
	}

	const CInputSystem& CInputComponent::GetInputSystem() const noexcept
	{
		return GetEngine().GetInputSystem();
	}

	void CInputComponent::ProcessActions() const
	{
		auto& inputSystem = GetInputSystem();
		for (auto& event : inputSystem.GetEvents())
		{
			auto [it, end] = actions_[event.bPressed].equal_range(event.name);
			for (; it != end; ++it)
			{
				it->second();
			}
		}
	}

	void CInputComponent::ProcessAxises() const
	{
		auto& inputSystem = GetInputSystem();
		for (auto& axisMap : axises_)
		{
			axisMap.second(inputSystem.GetAxisValue(axisMap.first));
		}
	}
}
