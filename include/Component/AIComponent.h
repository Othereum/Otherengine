#pragma once
#include <memory>
#include <unordered_map>
#include "Component/ActorComponent.h"
#include "Name.h"

namespace game
{
	namespace ai_state
	{
		class Base
		{
		public:
			virtual ~Base() = default;
			virtual void OnEnter(Base& prev) {}
			virtual void Update(float delta_seconds) {}
			virtual void OnExit(Base& next) {}
			[[nodiscard]] virtual FName GetName() const = 0;
		};
	}
	
	class AIComponent : public CActorComponent
	{
	public:
		AIComponent(AActor& owner, int update_order = 100);

	private:
		std::unordered_map<FName, std::unique_ptr<ai_state::Base>> states_;
		ai_state::Base& cur_;
	};
}
