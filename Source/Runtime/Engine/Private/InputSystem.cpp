#include "InputSystem.hpp"
#include <SDL2/SDL_events.h>
#include "Math.hpp"

namespace oeng
{
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
				InputAction{ConBtn(e.cbutton.button), KeyMod(SDL_GetModState())},
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

	InputSystem::InputSystem()
	{
		if (0 != SDL_SetRelativeMouseMode(SDL_TRUE))
			throw std::runtime_error{SDL_GetError()};
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

	void InputSystem::ClearEvents()
	{
		events_.clear();
	}

	void InputSystem::PostAddAllEvents()
	{
		int x, y;
		SDL_GetRelativeMouseState(&x, &y);
		mouse_.x = static_cast<float>(x);
		mouse_.y = static_cast<float>(y);
	}

	void InputSystem::AddAxis(Name name, DyArr<InputAxis>&& keys)
	{
		axises_.emplace(name, std::move(keys));
	}

	void InputSystem::AddAction(Name name, DyArr<InputAction>&& keys)
	{
		actions_.emplace(name, std::move(keys));
	}

	Float InputSystem::GetAxisValue(Name name) const
	{
		auto it = axises_.find(name);
		if (it == axises_.end()) return 0;

		auto val = 0.f;
		for (const auto& axis : it->second)
		{
			val += GetAxisValue(axis);
		}
		return val;
	}

	template <class... Ts>
	struct Overload : Ts... { using Ts::operator()...; };

	template <class... Ts>
	Overload(Ts...) -> Overload<Ts...>;

	Float InputSystem::GetAxisValue(const InputAxis& axis) const
	{
		return std::visit(Overload{
			
			[&](Keycode code)
			{
				const auto scan = SDL_GetScancodeFromKey(static_cast<int>(code));
				return SDL_GetKeyboardState(nullptr)[scan] ? axis.scale : 0;
			},
			
			[&](MouseBtn code)
			{
				const auto cur_btn = SDL_GetMouseState(nullptr, nullptr);
				return cur_btn & MouseMask(code) ? axis.scale : 0;
			},
			
			[&](ConBtn code)
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
			
			[&](ConAxis code)
			{
				constexpr auto min = 328_f, max = 32440_f;
				const auto v = SDL_GameControllerGetAxis(nullptr, SDL_GameControllerAxis(code));
				return v >= 0 ? MapRngClamp({min, max}, {0, 1}, v) : MapRngClamp({-max, -min}, {-1, 0}, v);
			}
			
		}, axis.code);
	}
}
