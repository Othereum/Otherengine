#pragma once
#include <variant>
#include "Name.hpp"
#include "Templates/DyArr.hpp"
#include "Templates/HashMap.hpp"
#include "InputCode.hpp"

union SDL_Event;

namespace oeng
{
	struct InputAxis
	{
		InputCode code = Keycode::UNKNOWN;
		Float scale = 1;
	};

	struct InputAction
	{
		InputCode code = Keycode::UNKNOWN;
		KeyMod mod = KeyMod::NONE;
	};

	class OEAPI InputSystem
	{
	public:
		InputSystem();
		
		void AddEvent(const SDL_Event& e);
		void ClearEvents();
		void PostAddAllEvents();

		void AddAxis(Name name, DyArr<InputAxis>&& keys);
		void AddAction(Name name, DyArr<InputAction>&& keys);
		
		[[nodiscard]] Float GetAxisValue(Name name) const noexcept;
		[[nodiscard]] Float GetAxisValue(const InputAxis& axis) const noexcept;
		[[nodiscard]] auto& GetEvents() const noexcept { return events_; }
		[[nodiscard]] auto& GetAxises() const noexcept { return axises_; }
		[[nodiscard]] auto& GetActions() const noexcept { return actions_; }

	private:
		struct InputEvent
		{
			Name name;
			bool pressed;
		};

		DyArr<InputEvent> events_;
		HashMap<Name, DyArr<InputAxis>> axises_;
		HashMap<Name, DyArr<InputAction>> actions_;
		struct { float x, y; } mouse_{};
	};
}
