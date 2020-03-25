#include "Component/AIComponent.h"

namespace game
{
	namespace ai_state
	{
		class Default : public Base
		{
		public:
			Default(): Base{nullptr} {}
			[[nodiscard]] FName GetName() const override { return "Default"; }
		};

		static Default default_state;
	}

	AIComponent::AIComponent(AActor& owner, int update_order):
		CActorComponent{owner, update_order},
		cur_{ai_state::default_state}
	{
	}

	void AIComponent::Update(float delta_seconds)
	{
		cur_.Update(delta_seconds);
	}
}
