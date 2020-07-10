#pragma once
#include "ActorComponent.hpp"
#include "Name.hpp"
#include "Templates/Function.hpp"
#include "Templates/HashMap.hpp"

struct SDL_KeyboardEvent;

namespace oeng
{
	class InputSystem;
	
	class OEAPI InputComponent : public ActorComponent
	{
	public:
		explicit InputComponent(class AActor& owner, int update_order = 0);
		
		void BindAction(Name action, bool pressed, Function<void()>&& callback);
		void BindAxis(Name axis, Function<void(Float)>&& callback);

		[[nodiscard]] const InputSystem& GetInputSystem() const noexcept;
		
	private:
		void OnUpdate(Float delta_seconds) override;
		
		void ProcessActions() const;
		void ProcessAxises() const;
		
		HashMultiMap<Name, Function<void()>> actions_[2];
		HashMultiMap<Name, Function<void(Float)>> axises_;
	};
}
