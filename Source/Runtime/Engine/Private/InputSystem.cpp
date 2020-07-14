#include "InputSystem.hpp"
#include <SDL_events.h>
#include "Debug.hpp"
#include "Engine.hpp"
#include "Math.hpp"

namespace oeng
{
	template <class... Ts>
	struct Overload : Ts... { using Ts::operator()...; };

	template <class... Ts>
	Overload(Ts...) -> Overload<Ts...>;

	static KeyMod GetModState() noexcept
	{
		auto mod = static_cast<int>(SDL_GetModState());
		if (mod & KMOD_SHIFT) mod |= KMOD_SHIFT;
		if (mod & KMOD_CTRL) mod |= KMOD_CTRL;
		if (mod & KMOD_ALT) mod |= KMOD_ALT;
		if (mod & KMOD_GUI) mod |= KMOD_GUI;
		return KeyMod(mod);
	}

	static std::optional<ParsedEvent> ParseEvent(const SDL_Event& e)
	{
		switch (e.type)
		{
		case SDL_KEYDOWN: case SDL_KEYUP:
			if (e.key.repeat) return std::nullopt;
			return ParsedEvent(Keycode(e.key.keysym.sym), !!e.key.state);

		case SDL_MOUSEBUTTONDOWN: case SDL_MOUSEBUTTONUP:
			return ParsedEvent(MouseBtn(e.button.button), !!e.button.state);

		case SDL_CONTROLLERBUTTONDOWN: case SDL_CONTROLLERBUTTONUP:
			return ParsedEvent(CtrlBtn(e.cbutton.button), !!e.cbutton.state);

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

	InputCode ToInputCode(const Json& json)
	{
		static const std::unordered_map<std::string_view, std::function<std::optional<InputCode>(std::u8string_view)>> parsers
		{
			{"Keycode"sv, ToKeycode},
			{"MouseBtn"sv, ToMouseBtn},
			{"CtrlBtn"sv, ToCtrlBtn},
			{"MouseAxis"sv, ToMouseAxis},
			{"CtrlAxis"sv, ToCtrlAxis},
		};

		const auto name = AsString8(json.at("Code").get<std::string>());
		const auto type = json.at("Type").get<std::string>();
		try
		{
			return parsers.at(type)(name).value();
		}
		catch (const std::out_of_range&)
		{
			Throw(u8"Invalid type '{}' (available: Keycode, MouseBtn, CtrlBtn, MouseAxis, CtrlAxis)", AsString8(type));
		}
		catch (const std::bad_optional_access&)
		{
			Throw(u8"Invalid code '{}'", name);
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
		
		if (auto mods = GetNames(action.mod); mods.empty())
		{
			json.erase("Mods");
		}
		else
		{
			auto& out = json["Mods"];
			for (const auto mod : mods) out.emplace_back(AsString(mod));
		}
	}

	template <class Map>
	static void LoadInput(const Json& config, const char* key, Map& mapped)
	{
		const auto map = config.find(key);
		if (map == config.end()) return;

		for (auto& [name, inputs] : map->items())
		{
			auto& arr = mapped[AsString8(name)];
			const auto size = inputs.size();
			for (size_t i=0; i<size; ++i) try
			{
				arr.emplace_back(inputs[i]);
			}
			catch (const std::exception& e)
			{
				log::Error(u8"Failed to load input mapping {}.{}[{}]: {}",
					AsString8(key), AsString8(name), i, What(e));
			}
		}
	}

	InputAxis::InputAxis(const Json& json)
		:code{ToInputCode(json)}, scale{json.at("Scale")}
	{
	}

	InputAction::InputAction(const Json& json)
		:code{ToInputCode(json)}, mod{KeyMod::NONE}
	{
		if (const auto mods_in = json.find("Mods"); mods_in != json.end())
		{
			for (auto& mod_in : mods_in.value()) try
			{
				mod |= ToKeyMod(AsString8(mod_in.get<std::string>())).value();
			}
			catch (const std::bad_optional_access&)
			{
				Throw(u8"Invalid mod '{}'", AsString8(mod_in.get<std::string>()));
			}
		}
	}

	InputSystem::InputSystem(Engine& engine)
		:engine_{engine}
	{
		SHOULD(0 == SDL_SetRelativeMouseMode(SDL_TRUE), AsString8(SDL_GetError()));

		const auto& config = engine.Config(u8"Input");
		LoadInput(config, "ActionMap", actions_);
		LoadInput(config, "AxisMap", axises_);
	}

	void InputSystem::AddEvent(const SDL_Event& e)
	{
		if (auto event = ParseEvent(e))
			AddEvent(*event);
	}

	void InputSystem::PostAddAllEvents()
	{
		int x, y;
		SDL_GetRelativeMouseState(&x, &y);
		mouse_.x = static_cast<float>(x);
		mouse_.y = static_cast<float>(y);

		auto trigger = [&](bool& old, CtrlAxis axis)
		{
			const auto pressed = GetAxisValue(axis) > 0.1_f;
			if (old != pressed) AddEvent({axis, pressed});
			old = pressed;
		};
		trigger(ctrl_.l, CtrlAxis::LT);
		trigger(ctrl_.r, CtrlAxis::RT);
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

	Float InputSystem::GetAxisValue(InputAxis axis) const
	{
		auto val = std::visit(Overload{
			
			[&](Keycode code)
			{
				const auto scan = SDL_GetScancodeFromKey(SDL_Keycode(code));
				return SDL_GetKeyboardState(nullptr)[scan] ? 1_f : 0_f;
			},
			
			[&](MouseBtn code)
			{
				const auto btn = SDL_GetMouseState(nullptr, nullptr);
				return btn & MouseMask(code) ? 1_f : 0_f;
			},
			
			[&](CtrlBtn code)
			{
				const auto btn = SDL_GameControllerButton(code);
				return SDL_GameControllerGetButton(nullptr, btn) ? 1_f : 0_f;
			},
			
			[&](MouseAxis code)
			{
				switch (code)
				{
				case MouseAxis::X: return mouse_.x;
				case MouseAxis::Y: return mouse_.y;
				default: return 0_f;
				}
			},
			
			[&](CtrlAxis code)
			{
				const auto ax = SDL_GameControllerAxis(code);
				const auto v = SDL_GameControllerGetAxis(nullptr, ax);
				return ToFloat(v) / (v >= 0 ? 32767_f : 32768_f);
			}
			
		}, axis.code);

		const auto config = axis_configs_.find(axis.code);
		if (config != axis_configs_.end())
		{
			auto& cfg = config->second;
			
			const auto dead = cfg.dead_zone / 2;
			val = val >= 0
				? Max(0, MapRng({dead, 1-dead}, {0, 1}, val))
				: Min(0, MapRng({dead-1, -dead}, {-1, 0}, val));

			val *= cfg.sensitivity;
			val = std::pow(val, cfg.exponent);
			if (cfg.invert) val = -val;
		}

		return val * axis.scale;
	}

	ParsedEvent::ParsedEvent(InputCode code, bool pressed)
		:input{code, GetModState()}, pressed{pressed}
	{
	}

	void InputSystem::AddEvent(ParsedEvent e)
	{
		for (const auto& act : actions_)
			if (IsMatch(e.input, act.second))
				events_.push_back({act.first, e.pressed});
	}

	void InputSystem::SaveConfig()
	{
		const Name conf_name = u8"Input";
		auto& config = engine_.Config(conf_name);
		auto save = [&](const char* key, const auto& mapped)
		{
			auto& map = config[key];
			for (auto& [name, inputs] : mapped)
			{
				auto& out_inputs = map[AsString(*name)] = {};
				for (auto& input : inputs) out_inputs.emplace_back(input);
			}
		};
		save("ActionMap", actions_);
		save("AxisMap", axises_);
		engine_.SaveConfig(conf_name);
	}
}
