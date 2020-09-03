#pragma once
#include "Components/ActorComponent.hpp"

namespace oeng
{
inline namespace engine
{
namespace ai_state
{
class Base;
}

class ENGINE_API AIComponent : public ActorComponent
{
CLASS_BODY(AIComponent)

public:
    AIComponent();

    /**
     * Change state. You must add state before change, or std::out_of_bound exception will be thrown
     * @param name New state name
     */
    void ChangeState(Name name);

    void AddState(std::unique_ptr<ai_state::Base> state);

    template <class T, class... Args>
    void AddState(Args&&... args)
    {
        AddState(std::make_unique<T>(std::forward<Args>(args)...));
    }

private:
    void OnUpdate(Float delta_seconds) override;

    std::unordered_map<Name, std::unique_ptr<ai_state::Base>> states_;
    ai_state::Base* cur_;
};
}
}
