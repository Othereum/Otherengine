#pragma once
#include <variant>
#include "Name.hpp"
#include "Templates/DyArr.hpp"
#include "Templates/HashMap.hpp"
#include "InputCode.hpp"

union SDL_Event;

namespace oeng
{
	class Engine;

	using InputCode = std::variant<Keycode, MouseBtn, CtrlBtn, MouseAxis, CtrlAxis>;

	struct InputAxis
	{
		template <std::convertible_to<InputCode> T>
		constexpr InputAxis(T code, Float scale) noexcept
			:code{code}, scale{scale}
		{
		}
		
		InputCode code;
		Float scale;
	};

	struct InputAction
	{
		template <std::convertible_to<InputCode> T>
		constexpr InputAction(T code, KeyMod mod = KeyMod::NONE) noexcept
			:code{code}, mod{mod}
		{
		}
		
		InputCode code;
		KeyMod mod;
	};

	class OEAPI InputSystem
	{
	public:
		explicit InputSystem(Engine& engine);
		
		void AddEvent(const SDL_Event& e);
		void ClearEvents() { events_.clear(); }
		void PostAddAllEvents();

		void AddAxis(Name name, DyArr<InputAxis>&& keys)
		{ axises_.emplace(name, std::move(keys)); }
		
		void AddAction(Name name, DyArr<InputAction>&& keys)
		{ actions_.emplace(name, std::move(keys)); }
		
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
