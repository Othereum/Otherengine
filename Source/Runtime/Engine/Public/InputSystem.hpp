#pragma once
#include <vector>
#include <unordered_map>
#include "Name.hpp"

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

	class InputSystem
	{
	public:
		void AddEvent(const SDL_Event& e);
		void ClearEvents();

		void AddAxis(Name name, std::vector<InputAxis>&& keys);
		void AddAction(Name name, std::vector<InputAction>&& keys);
		
		[[nodiscard]] Float GetAxisValue(Name name) const noexcept;
		[[nodiscard]] static Float GetAxisValue(const InputAxis& axis) noexcept;
		[[nodiscard]] auto& GetEvents() const noexcept { return events_; }
		[[nodiscard]] auto& GetAxises() const noexcept { return axises_; }
		[[nodiscard]] auto& GetActions() const noexcept { return actions_; }

	private:
		std::vector<InputEvent> events_;
		std::unordered_map<Name, std::vector<InputAxis>> axises_;
		std::unordered_map<Name, std::vector<InputAction>> actions_;
	};
}
