#include "AI/AIComponent.hpp"
#include "AI/AIState.hpp"

ENGINE_BEGIN

namespace ai_state
{
	class Default : public Base
	{
	public:
		Default(): Base{nullptr} {}
		[[nodiscard]] Name GetName() const override { return u8"Default"sv; }
	};

	static Default default_state;
}

AIComponent::AIComponent(AActor& owner, int update_order):
	ActorComponent{owner, update_order},
	cur_{ai_state::default_state}
{
}

void AIComponent::OnUpdate(Float delta_seconds)
{
	cur_.get().Update(delta_seconds);
}

void AIComponent::ChangeState(Name name)
{
	auto& prev = cur_.get();
	auto& next = *states_.at(name);
	prev.OnExit(next);
	cur_ = next;
	next.OnEnter(prev);
}

void AIComponent::AddState(UniquePtr<ai_state::Base>&& state)
{
	states_.emplace(state->GetName(), std::move(state));
}

ENGINE_END
