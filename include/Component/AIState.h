#pragma once
#include "Name.h"

namespace game::ai_state
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
