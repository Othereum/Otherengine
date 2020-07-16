#pragma once
#include <functional>
#include "ActorComponent.hpp"
#include "Name.hpp"
#include "Templates/HashMap.hpp"

struct SDL_KeyboardEvent;

namespace oeng::engine
{
	class InputSystem;
	
	class OEAPI InputComponent : public ActorComponent
	{
	public:
		explicit InputComponent(class AActor& owner, int update_order = 0);
		
		void BindAction(Name action, bool pressed, std::function<void()>&& callback);
		void BindAxis(Name axis, std::function<void(Float)>&& callback);

		[[nodiscard]] const InputSystem& GetInputSystem() const noexcept;
		
	private:
		void OnUpdate(Float delta_seconds) override;
		
		void ProcessActions() const;
		void ProcessAxises() const;
		
		HashMultiMap<Name, std::function<void()>> actions_[2];
		HashMultiMap<Name, std::function<void(Float)>> axises_;
	};
}
