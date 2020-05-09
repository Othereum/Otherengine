#include "InputSystem.hpp"
#include <SDL_events.h>
#include "Math.hpp"

namespace oeng
{
	static uint16_t GetModState(uint16_t mod)
	{
		return mod & (KMOD_CTRL | KMOD_SHIFT | KMOD_ALT);
	}
	
	static uint16_t GetModState()
	{
		return GetModState(SDL_GetModState());
	}

	static bool ParseEvent(FInputAction& key_event, bool& pressed, const SDL_Event& e)
	{
		switch (e.type)
		{
		case SDL_KEYDOWN: case SDL_KEYUP:
			key_event.key = e.key.keysym.sym;
			key_event.type = EInputType::keyboard;
			key_event.mod = GetModState(e.key.keysym.mod);
			pressed = e.key.state;
			break;

		case SDL_MOUSEBUTTONDOWN: case SDL_MOUSEBUTTONUP:
			key_event.key = e.button.button;
			key_event.type = EInputType::mButton;
			key_event.mod = GetModState();
			pressed = e.button.state;
			break;

		case SDL_CONTROLLERBUTTONDOWN: case SDL_CONTROLLERBUTTONUP:
			key_event.key = e.cbutton.button;
			key_event.type = EInputType::cButton;
			key_event.mod = GetModState();
			pressed = e.cbutton.state;
			break;

		default:
			return false;
		}

		return true;
	}

	static bool IsMatch(const FInputAction& event, const std::vector<FInputAction>& keys)
	{
		for (const auto& key : keys)
		{
			if (event == key && (event.mod & key.mod) == key.mod)
			{
				return true;
			}
		}
		
		return false;
	}

	void CInputSystem::AddEvent(const SDL_Event& e)
	{
		FInputAction key_event;
		bool pressed;
		if (!ParseEvent(key_event, pressed, e)) return;
		
		for (const auto& act : actions_)
		{
			if (IsMatch(key_event, act.second))
			{
				events_.push_back({act.first, pressed});
			}
		}
	}

	void CInputSystem::ClearEvents()
	{
		events_.clear();
	}

	void CInputSystem::AddAxis(Name name, std::vector<FInputAxis>&& keys)
	{
		axises_.emplace(name, std::move(keys));
	}

	void CInputSystem::AddAction(Name name, std::vector<FInputAction>&& keys)
	{
		actions_.emplace(name, std::move(keys));
	}

	float CInputSystem::GetAxisValue(Name name) const noexcept
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

	float CInputSystem::GetAxisValue(const FInputAxis& axis) noexcept
	{
		switch (axis.type)
		{
		case EInputType::keyboard:
			return SDL_GetKeyboardState(nullptr)[SDL_GetScancodeFromKey(axis.key)] ? axis.scale : 0;
			
		case EInputType::mButton:
			return SDL_GetMouseState(nullptr, nullptr) & SDL_BUTTON(axis.key) ? axis.scale : 0;
			
		case EInputType::mAxisX:
			{
				int x;
				SDL_GetRelativeMouseState(&x, nullptr);
				return static_cast<float>(x) * axis.scale;
			}
			
		case EInputType::mAxisY:
			{
				int y;
				SDL_GetRelativeMouseState(nullptr, &y);
				return static_cast<float>(y) * axis.scale;
			}
			
		case EInputType::cButton:
			return SDL_GameControllerGetButton(nullptr, SDL_GameControllerButton(axis.key)) ? axis.scale : 0;
			
		case EInputType::cAxis:
			{
				constexpr float min = 328, max = 32440;
				const auto v = SDL_GameControllerGetAxis(nullptr, SDL_GameControllerAxis(axis.key));
				return v >= 0 ? MapRngClamp({min, max}, {0, 1}, v) : MapRngClamp({-max, -min}, {-1, 0}, v);
			}
			
		default:
			return 0;
		}
	}
}
