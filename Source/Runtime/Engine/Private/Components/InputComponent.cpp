#include "Components/InputComponent.hpp"
#include "Engine/Engine.hpp"
#include "Input/InputSystem.hpp"

namespace oeng
{
inline namespace engine
{

InputComponent::InputComponent() : ActorComponent{0}
{
}

void InputComponent::OnUpdate(Float)
{
    if (IsActive())
    {
        ProcessActions();
        ProcessAxises();
    }
}

void InputComponent::BindAction(Name action, bool pressed, std::function<void()>&& callback)
{
    actions_[pressed].emplace(action, std::move(callback));
}

void InputComponent::BindAxis(Name axis, std::function<void(Float)>&& callback)
{
    axises_.emplace(axis, std::move(callback));
}

void InputComponent::ProcessActions() const
{
    for (const auto& event : GetEngine().input_system.GetEvents())
    {
        auto [it, end] = actions_[event.pressed].equal_range(event.name);
        for (; it != end; ++it)
            it->second();
    }
}

void InputComponent::ProcessAxises() const
{
    const auto& input_system = GetEngine().input_system;
    for (const auto& axis_map : axises_)
    {
        axis_map.second(input_system.GetAxisValue(axis_map.first));
    }
}

} // namespace engine
} // namespace oeng
