#include "Components/AIComponent.hpp"
#include "Components/AIState.hpp"

namespace oeng
{
	namespace ai_state
	{
		class Default : public Base
		{
		public:
			Default(): Base{nullptr} {}
			[[nodiscard]] Name GetName() const override { return "Default"; }
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

	void AIComponent::AddState(std::unique_ptr<ai_state::Base>&& state)
	{
		states_.emplace(state->GetName(), std::move(state));
	}
}
