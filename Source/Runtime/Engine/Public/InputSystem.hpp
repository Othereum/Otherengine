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
		using Code = std::variant<Keycode, MouseBtn, ConBtn, MouseAxis, ConAxis>;

		template <std::convertible_to<Code> T>
		constexpr InputAxis(T code, Float scale) noexcept
			:code{code}, scale{scale}
		{
		}
		
		Code code;
		Float scale;
	};

	struct InputAction
	{
		using Code = std::variant<Keycode, MouseBtn, ConBtn>;

		template <std::convertible_to<Code> T>
		constexpr InputAction(T code, KeyMod mod = KeyMod::NONE) noexcept
			:code{code}, mod{mod}
		{
		}
		
		Code code;
		KeyMod mod;
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
		
		[[nodiscard]] Float GetAxisValue(Name name) const;
		[[nodiscard]] Float GetAxisValue(const InputAxis& axis) const;
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
		struct { Float x, y; } mouse_{};
	};
}
