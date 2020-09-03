#include "AI/AIComponent.hpp"
#include "AI/AIState.hpp"

namespace oeng
{
inline namespace engine
{
namespace ai_state
{
class Default final : public Base
{
public:
    Default()
        : Base{nullptr}
    {
    }

    [[nodiscard]] Name GetName() const override
    {
        return u8"Default"sv;
    }
};

static Default default_state;
}

AIComponent::AIComponent()
    : cur_{&ai_state::default_state}
{
}

void AIComponent::OnUpdate(Float delta_seconds)
{
    cur_->Update(delta_seconds);
}

void AIComponent::ChangeState(Name name)
{
    auto& prev = *cur_;
    auto& next = *states_.at(name);
    prev.OnExit(next);
    cur_ = &next;
    next.OnEnter(prev);
}

void AIComponent::AddState(std::unique_ptr<ai_state::Base> state)
{
    states_.emplace(state->GetName(), std::move(state));
}
}
}
