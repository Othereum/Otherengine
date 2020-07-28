#pragma once
#include <variant>
#include "InputCode.hpp"
#include "Math.hpp"
#include "Name.hpp"
#include "Templates/DyArr.hpp"
#include "Templates/HashMap.hpp"
#include "Templates/Pointer.hpp"

namespace oeng::engine
{
	using InputCode = std::variant<Keycode, MouseBtn, CtrlBtn, MouseAxis, CtrlAxis>;
	using CtrlPtr = UniquePtr<_SDL_GameController, void(*)(_SDL_GameController*)>;
	
	struct ENGINE_API InputAxis
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

	struct ENGINE_API InputAction
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

	struct AxisConfig
	{
		Float dead_zone = 0_f;
		Float sensitivity = 1_f;
	};

	struct ENGINE_API ParsedEvent
	{
		ParsedEvent(InputCode code, bool pressed);
		InputAction input;
		bool pressed;
	};

	ENGINE_API String ToString(InputCode code);
	ENGINE_API InputCode ToInputCode(std::string_view str);
	ENGINE_API void to_json(Json& json, const InputAxis& axis);
	ENGINE_API void to_json(Json& json, const InputAction& action);
	ENGINE_API void to_json(Json& json, const AxisConfig& action);
	ENGINE_API void from_json(const Json& json, AxisConfig& action);

	class ENGINE_API InputSystem
	{
	public:
		DELETE_CPMV(InputSystem);

		[[nodiscard]] static InputSystem& Get() noexcept;
		
		InputSystem();
		~InputSystem() = default;
		
		void AddEvent(const SDL_Event& e);
		void AddEvent(ParsedEvent e);
		void ClearEvents() { events_.clear(); }
		void PostAddAllEvents();
		void SaveConfig();

		[[nodiscard]] Float GetAxisValue(Name name) const;
		[[nodiscard]] Float GetAxisValue(InputAxis axis) const;
		[[nodiscard]] Float GetAxisValue(CtrlBtn code) const noexcept;
		[[nodiscard]] Float GetAxisValue(CtrlAxis code) const noexcept;
		[[nodiscard]] Float GetAxisValue(MouseAxis code) const noexcept;
		[[nodiscard]] Float GetAxisValue(MouseBtn code) const noexcept;
		[[nodiscard]] Float GetAxisValue(Keycode code) const noexcept;
		
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

		void AddController(int id);
		void RemoveController(int id);

		[[nodiscard]] Float FilterAxis(InputCode code, Float val) const noexcept;
		[[nodiscard]] Vec2 FilterAxis(InputCode code, Vec2 val) const noexcept;
		[[nodiscard]] _SDL_GameController* Ctrl() const noexcept;
		
		DyArr<InputEvent> events_;
		
		Vec2 mouse_{};
		struct { bool l, r; } trig_{};
		
		DyArr<CtrlPtr> ctrls_;
	};
}
