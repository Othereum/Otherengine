#pragma once
#include "ActorComponent.hpp"

struct SDL_KeyboardEvent;

namespace oeng
{
inline namespace engine
{

class InputSystem;

class ENGINE_API InputComponent final : public ActorComponent
{
    CLASS_BODY(InputComponent)

  public:
    InputComponent();

    void BindAction(Name action, bool pressed, std::function<void()>&& callback);
    void BindAxis(Name axis, std::function<void(Float)>&& callback);

  protected:
    void OnUpdate(Float delta_seconds) override;

  private:
    void ProcessActions() const;
    void ProcessAxises() const;

    std::unordered_multimap<Name, std::function<void()>> actions_[2];
    std::unordered_multimap<Name, std::function<void(Float)>> axises_;
};

} // namespace engine
} // namespace oeng
