#pragma once
#include <memory>
#include <unordered_map>
#include "ActorComponent.h"
#include "Name.h"

namespace game
{
	namespace ai_state
	{
		class Base
		{
		public:
			explicit Base(class AIComponent& owner) noexcept: owner_{owner} {}
			explicit Base(class AIComponent* owner) noexcept: owner_{*owner} {}
			virtual ~Base() = default;
			virtual void OnEnter(Base& prev) {}
			virtual void Update(float delta_seconds) {}
			virtual void OnExit(Base& next) {}
			[[nodiscard]] virtual FName GetName() const = 0;
			[[nodiscard]] AIComponent& GetOwner() const noexcept { return owner_; }

		private:
			AIComponent& owner_;
		};
	}
	
	class AIComponent : public CActorComponent
	{
	public:
		AIComponent(AActor& owner, int update_order = 100);
		void Update(float delta_seconds) override;

	private:
		std::unordered_map<FName, std::unique_ptr<ai_state::Base>> states_;
		ai_state::Base& cur_;
	};
}
