#include "InputSystem.h"
#include <SDL_events.h>

namespace Game
{
	void CInputSystem::AddEvent(const SDL_Event& e)
	{
		FInputEvent i;
		
		switch (e.type)
		{
		case SDL_KEYDOWN: case SDL_KEYUP:
			i.key.key = e.key.keysym.sym;
			i.key.mod = e.key.keysym.mod;
			i.key.type = EInputType::keyboard;
			i.bPressed = e.key.state;
			break;

		case SDL_MOUSEBUTTONDOWN: case SDL_MOUSEBUTTONUP:
			i.key.key = e.button.button;
			i.key.mod = SDL_GetModState();
			i.key.type = EInputType::mouse;
			i.bPressed = e.button.state;
			break;

		case SDL_CONTROLLERBUTTONDOWN: case SDL_CONTROLLERBUTTONUP:
			i.key.key = e.cbutton.button;
			i.key.mod = SDL_GetModState();
			i.key.type = EInputType::controller;
			i.bPressed = e.cbutton.state;
			break;

		default:
			return;
		}

		events_.push_back(i);
	}

	void CInputSystem::ClearEvents()
	{
		events_.clear();
	}
}
