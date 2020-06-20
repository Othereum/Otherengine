#pragma once
#include "Name.hpp"
#include "Templates/DyArr.hpp"
#include "Templates/HashMap.hpp"

union SDL_Event;

namespace oeng
{	
	enum class InputType : int8_t
	{
		kInvalid = -1, kKeyboard, kMButton, kMAxisX, kMAxisY, kCButton, kCAxis
	};

	struct InputKey
	{
		int key = -1;
		InputType type = InputType::kInvalid;
		
		constexpr InputKey() noexcept = default;
		constexpr InputKey(int key, InputType type) noexcept: key{key}, type{type} {}
		
		bool operator==(const InputKey&) const noexcept = default;
	};

	struct InputAxis : InputKey
	{
		Float scale = 1;
		
		constexpr InputAxis() noexcept = default;
		constexpr InputAxis(int key, InputType type, Float scale) noexcept: InputKey{key, type}, scale{scale} {}
	};

	struct InputAction : InputKey
	{
		uint16_t mod = 0;

		constexpr InputAction() noexcept = default;
		constexpr InputAction(int key, InputType type, uint16_t mod = 0) noexcept: InputKey{key, type}, mod{mod} {}
	};

	struct InputEvent
	{
		Name name;
		bool pressed;
	};

	class OEAPI InputSystem
	{
	public:
		InputSystem();
		
		void AddEvent(const SDL_Event& e);
		void ClearEvents();
		void PostAddAllEvents();

		void AddAxis(Name name, DyArr<InputAxis>&& keys);
		void AddAction(Name name, DyArr<InputAction>&& keys);
		
		[[nodiscard]] Float GetAxisValue(Name name) const noexcept;
		[[nodiscard]] Float GetAxisValue(const InputAxis& axis) const noexcept;
		[[nodiscard]] auto& GetEvents() const noexcept { return events_; }
		[[nodiscard]] auto& GetAxises() const noexcept { return axises_; }
		[[nodiscard]] auto& GetActions() const noexcept { return actions_; }

	private:
		DyArr<InputEvent> events_;
		HashMap<Name, DyArr<InputAxis>> axises_;
		HashMap<Name, DyArr<InputAction>> actions_;
		struct { float x, y; } mouse_{};
	};
}
