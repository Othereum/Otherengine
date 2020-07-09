#include "InputSystem.hpp"
#include <SDL2/SDL_events.h>
#include "Debug.hpp"
#include "Engine.hpp"
#include "Math.hpp"

namespace oeng
{
	template <class... Ts>
	struct Overload : Ts... { using Ts::operator()...; };

	template <class... Ts>
	Overload(Ts...) -> Overload<Ts...>;

	struct ParsedEvent
	{
		InputAction input;
		bool pressed;
	};
	
	static std::optional<ParsedEvent> ParseEvent(const SDL_Event& e)
	{
		switch (e.type)
		{
		case SDL_KEYDOWN: case SDL_KEYUP:
			if (e.key.repeat) return std::nullopt;
			return ParsedEvent{
				InputAction{Keycode(e.key.keysym.sym), KeyMod(e.key.keysym.mod)},
				!!e.key.state
			};

		case SDL_MOUSEBUTTONDOWN: case SDL_MOUSEBUTTONUP:
			return ParsedEvent{
				InputAction{MouseBtn(e.button.button), KeyMod(SDL_GetModState())},
				!!e.button.state
			};

		case SDL_CONTROLLERBUTTONDOWN: case SDL_CONTROLLERBUTTONUP:
			return ParsedEvent{
				InputAction{CtrlBtn(e.cbutton.button), KeyMod(SDL_GetModState())},
				!!e.cbutton.state
			};

		default:
			return std::nullopt;
		}
	}

	static bool IsMatch(const InputAction& event, const DyArr<InputAction>& keys)
	{
		for (const auto& key : keys)
			if (event.code == key.code && (event.mod & key.mod) == key.mod)
				return true;
		return false;
	}

	void from_json(const Json& json, InputCode& code)
	{
		static const std::unordered_map<std::string_view, std::function<InputCode(std::u8string_view)>> parsers
		{
			{"Keycode"sv, ToKeycode},
			{"MouseBtn"sv, ToMouseBtn},
			{"CtrlBtn"sv, ToCtrlBtn},
			{"MouseAxis"sv, ToMouseAxis},
			{"CtrlAxis"sv, ToCtrlAxis},
		};

		auto name = json.at("Code").get<std::string>();
		code = parsers.at(json.at("Type"))(AsString8(std::move(name)));
	}

	void from_json(const Json& json, InputAxis& axis)
	{
		axis.code = json;
		axis.scale = json.at("Scale");
	}

	void from_json(const Json& json, InputAction& action)
	{
		action.code = json;
		for (auto& mod : json.at("Mods"))
		{
			action.mod |= ToKeyMod(AsString8(mod));
		}
	}

	void to_json(Json& json, const InputCode& code)
	{
		json["Code"] = AsString(std::visit([](auto code) { return String8{GetName(code)}; }, code));
		json["Type"] = std::visit(Overload{
			[](Keycode) { return "Keycode"s; },
			[](MouseBtn) { return "MouseBtn"s; },
			[](CtrlBtn) { return "CtrlBtn"s; },
			[](MouseAxis) { return "MouseAxis"s; },
			[](CtrlAxis) { return "CtrlAxis"s; },
		}, code);
	}

	void to_json(Json& json, const InputAxis& axis)
	{
		json = axis.code;
		json["Scale"] = axis.scale;
	}

	void to_json(Json& json, const InputAction& action)
	{
		json = action.code;
		json["Mods"] = GetNames(action.mod);
	}

	InputSystem::InputSystem(Engine& engine)
	{
		SHOULD(0 != SDL_SetRelativeMouseMode(SDL_TRUE), AsString8(SDL_GetError()));
		
		auto& config = engine.Config(u8"Input");
		auto load = [&]<class T>(T&& key, auto& mapped)
		{
			const auto map = config.find(std::forward<T>(key));
			if (map == config.end()) return;
			
			for (auto& [name, inputs] : map->items())
			{
				auto& arr = mapped[AsString8(name)];
				for (auto& input : inputs) arr.emplace_back(input);
			}
		};
		load("ActionMap", actions_);
		load("AxisMap", axises_);
	}

	void InputSystem::AddEvent(const SDL_Event& e)
	{
		const auto event = ParseEvent(e);
		if (!event) return;
		
		for (const auto& act : actions_)
		{
			if (IsMatch(event->input, act.second))
			{
				events_.push_back({act.first, event->pressed});
			}
		}
	}

	void InputSystem::PostAddAllEvents()
	{
		int x, y;
		SDL_GetRelativeMouseState(&x, &y);
		mouse_.x = static_cast<float>(x);
		mouse_.y = static_cast<float>(y);
	}

	Float InputSystem::GetAxisValue(Name name) const
	{
		auto it = axises_.find(name);
		if (it == axises_.end()) return 0;

		auto val = 0_f;
		for (const auto& axis : it->second)
		{
			val += GetAxisValue(axis);
		}
		return val;
	}

	Float InputSystem::GetAxisValue(const InputAxis& axis) const
	{
		return std::visit(Overload{
			
			[&](Keycode code)
			{
				const auto scan = SDL_GetScancodeFromKey(SDL_Keycode(code));
				return SDL_GetKeyboardState(nullptr)[scan] ? axis.scale : 0;
			},
			
			[&](MouseBtn code)
			{
				const auto cur_btn = SDL_GetMouseState(nullptr, nullptr);
				return cur_btn & MouseMask(code) ? axis.scale : 0;
			},
			
			[&](CtrlBtn code)
			{
				const auto btn = SDL_GameControllerButton(code);
				return SDL_GameControllerGetButton(nullptr, btn) ? axis.scale : 0;
			},
			
			[&](MouseAxis code)
			{
				switch (code)
				{
				case MouseAxis::X: return mouse_.x * axis.scale;
				case MouseAxis::Y: return mouse_.y * axis.scale;
				default: return 0_f;
				}
			},
			
			[&](CtrlAxis code)
			{
				constexpr auto min = 328_f, max = 32440_f;
				const auto v = SDL_GameControllerGetAxis(nullptr, SDL_GameControllerAxis(code));
				return v >= 0 ? MapRngClamp({min, max}, {0, 1}, v) : MapRngClamp({-max, -min}, {-1, 0}, v);
			}
			
		}, axis.code);
	}
}
