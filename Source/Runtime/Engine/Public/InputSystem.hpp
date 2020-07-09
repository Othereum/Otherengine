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
		template <std::convertible_to<InputCode> T = Keycode>
		constexpr InputAxis(T code = Keycode::UNKNOWN, Float scale = 1.f) noexcept
			:code{code}, scale{scale}
		{
		}
		
		InputCode code;
		Float scale;
	};

	struct InputAction
	{
		template <std::convertible_to<InputCode> T = Keycode>
		constexpr InputAction(T code = Keycode::UNKNOWN, KeyMod mod = KeyMod::NONE) noexcept
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

		bool AddAxis(Name name, DyArr<InputAxis>&& keys)
		{
			return axises_.insert_or_assign(name, std::move(keys)).second;
		}
		
		bool AddAction(Name name, DyArr<InputAction>&& keys)
		{
			return actions_.insert_or_assign(name, std::move(keys)).second;
		}

		[[nodiscard]] Float GetAxisValue(Name name) const;
		[[nodiscard]] Float GetAxisValue(const InputAxis& axis) const;
		[[nodiscard]] Engine& GetEngine() const noexcept { return engine_; }
		[[nodiscard]] auto& GetEvents() const noexcept { return events_; }
		[[nodiscard]] auto& GetAxises() const noexcept { return axises_; }
		[[nodiscard]] auto& GetActions() const noexcept { return actions_; }

		void SaveConfig();

	private:
		struct InputEvent
		{
			Name name;
			bool pressed;
		};

		Engine& engine_;
		DyArr<InputEvent> events_;
		HashMap<Name, DyArr<InputAxis>> axises_;
		HashMap<Name, DyArr<InputAction>> actions_;
		struct { Float x, y; } mouse_{};
	};
}
