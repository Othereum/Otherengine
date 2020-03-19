#pragma once
#include <vector>

union SDL_Event;

namespace Game
{
	enum class EInputType : uint8_t
	{
		keyboard, mouse, controller
	};

	struct FKeyData
	{
		int key;
		uint16_t mod;
		EInputType type;
	};

	struct FInputEvent
	{
		FKeyData key;
		bool bPressed;
	};
	
	class CInputSystem
	{
	public:
		void AddEvent(const SDL_Event& e);
		void ClearEvents();

	private:
		std::vector<FInputEvent> events_;
	};
}
