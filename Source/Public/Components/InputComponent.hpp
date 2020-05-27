#pragma once
#include <functional>
#include "ActorComponent.hpp"
#include "Name.hpp"

struct SDL_KeyboardEvent;

namespace oeng
{
	class CInputComponent : public ActorComponent
	{
	public:
		explicit CInputComponent(class AActor& owner, int updateOrder = 1);
		
		void Update(float deltaSeconds) override;
		
		void BindAction(Name action, bool bPressed, std::function<void()>&& callback);
		void BindAxis(Name axis, std::function<void(float)>&& callback);

		[[nodiscard]] const class CInputSystem& GetInputSystem() const noexcept;
		
	private:
		void ProcessActions() const;
		void ProcessAxises() const;
		
		std::unordered_multimap<Name, std::function<void()>> actions_[2];
		std::unordered_multimap<Name, std::function<void(float)>> axises_;
	};
}
