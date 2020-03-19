#pragma once
#include <functional>
#include "ActorComponent.h"
#include "Name.h"

struct SDL_KeyboardEvent;

namespace Game
{
	class CInputComponent : public CActorComponent
	{
	public:
		explicit CInputComponent(class AActor& owner, int updateOrder = 1);
		
		void Update(float deltaSeconds) override;
		
		void BindAction(FName action, bool bPressed, std::function<void()>&& callback);
		void BindAxis(FName axis, std::function<void(float)>&& callback);

		[[nodiscard]] const class CInputSystem& GetInputSystem() const noexcept;
		
	private:
		void ProcessActions() const;
		void ProcessAxises() const;
		
		std::unordered_multimap<FName, std::function<void()>> actions_[2];
		std::unordered_multimap<FName, std::function<void(float)>> axises_;
	};
}
