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
		void SaveConfig();

		[[nodiscard]] Float GetAxisValue(Name name) const;
		[[nodiscard]] Float GetAxisValue(InputAxis axis) const;
		[[nodiscard]] Engine& GetEngine() const noexcept { return engine_; }
		[[nodiscard]] auto& GetEvents() const noexcept { return events_; }

		HashMap<Name, DyArr<InputAxis>> axises_;
		HashMap<Name, DyArr<InputAction>> actions_;
		
	private:
		struct InputEvent
		{
			Name name;
			bool pressed;
		};

		Engine& engine_;
		DyArr<InputEvent> events_;
		struct { Float x, y; } mouse_{};
	};
}
