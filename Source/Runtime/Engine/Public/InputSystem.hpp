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

	struct OEAPI InputAxis
	{
		template <std::convertible_to<InputCode> T>
		constexpr InputAxis(T code, Float scale = 1_f) noexcept
			:code{code}, scale{scale}
		{
		}

		explicit InputAxis(const Json& json);
		
		InputCode code;
		Float scale;
	};

	struct OEAPI InputAction
	{
		template <std::convertible_to<InputCode> T>
		constexpr InputAction(T code, KeyMod mod = KeyMod::NONE) noexcept
			:code{code}, mod{mod}
		{
		}
		
		explicit InputAction(const Json& json);
		
		InputCode code;
		KeyMod mod;
	};

	OEAPI InputCode ToInputCode(const Json& json);
	OEAPI void to_json(Json& json, const InputCode& code);
	OEAPI void to_json(Json& json, const InputAxis& axis);
	OEAPI void to_json(Json& json, const InputAction& action);

	struct OEAPI ParsedEvent
	{
		ParsedEvent(InputCode code, bool pressed);
		InputAction input;
		bool pressed;
	};

	struct AxisConfig
	{
		Float dead_zone = 0_f;
		Float sensitivity = 1_f;
		Float exponent = 1_f;
		bool invert = false;
	};

	class OEAPI InputSystem
	{
	public:
		explicit InputSystem(Engine& engine);
		
		void AddEvent(const SDL_Event& e);
		void AddEvent(ParsedEvent e);
		void ClearEvents() { events_.clear(); }
		void PostAddAllEvents();
		void SaveConfig();

		[[nodiscard]] Float GetAxisValue(Name name) const;
		[[nodiscard]] Float GetAxisValue(InputAxis axis) const;
		[[nodiscard]] Engine& GetEngine() const noexcept { return engine_; }
		[[nodiscard]] auto& GetEvents() const noexcept { return events_; }

		HashMap<Name, DyArr<InputAxis>> axises_;
		HashMap<Name, DyArr<InputAction>> actions_;
		HashMap<InputCode, AxisConfig> axis_configs_;
		
	private:
		struct InputEvent
		{
			Name name;
			bool pressed;
		};
		
		Engine& engine_;
		DyArr<InputEvent> events_;
		struct { Float x, y; } mouse_{};
		struct { bool l, r; } ctrl_{};
	};
}
