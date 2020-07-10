#pragma once
#include <functional>
#include "ActorComponent.hpp"
#include "Name.hpp"

struct SDL_KeyboardEvent;

namespace oeng
{
	enum class InputEvent { kRelease, kPress };
	
	class OEAPI InputComponent : public ActorComponent
	{
	public:
		explicit InputComponent(class AActor& owner, int update_order = 1);
		
		void BindAction(Name action, InputEvent pressed, std::function<void()>&& callback);
		void BindAxis(Name axis, std::function<void(Float)>&& callback);

		[[nodiscard]] const class InputSystem& GetInputSystem() const noexcept;
		
	private:
		void OnUpdate(Float delta_seconds) override;
		
		void ProcessActions() const;
		void ProcessAxises() const;
		
		std::unordered_multimap<Name, std::function<void()>> actions_[2];
		std::unordered_multimap<Name, std::function<void(Float)>> axises_;
	};
}
