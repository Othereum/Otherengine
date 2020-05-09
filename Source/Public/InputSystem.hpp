#pragma once
#include <vector>
#include <unordered_map>
#include "Name.hpp"

union SDL_Event;

namespace oeng
{	
	enum class EInputType : char
	{
		invalid = -1, keyboard, mButton, mAxisX, mAxisY, cButton, cAxis
	};

	struct FInputKey
	{
		int key = -1;
		EInputType type = EInputType::invalid;
		
		constexpr FInputKey() noexcept = default;
		constexpr FInputKey(int key, EInputType type) noexcept: key{key}, type{type} {}
		
		bool operator==(const FInputKey&) const noexcept = default;
	};

	struct FInputAxis : FInputKey
	{
		float scale = 1;
		
		constexpr FInputAxis() noexcept = default;
		constexpr FInputAxis(int key, EInputType type, float scale) noexcept: FInputKey{key, type}, scale{scale} {}
	};

	struct FInputAction : FInputKey
	{
		uint16_t mod = 0;

		constexpr FInputAction() noexcept = default;
		constexpr FInputAction(int key, EInputType type, uint16_t mod = 0) noexcept: FInputKey{key, type}, mod{mod} {}
	};

	struct FInputEvent
	{
		Name name;
		bool bPressed;
	};

	class CInputSystem
	{
	public:
		void AddEvent(const SDL_Event& e);
		void ClearEvents();

		void AddAxis(Name name, std::vector<FInputAxis>&& keys);
		void AddAction(Name name, std::vector<FInputAction>&& keys);
		
		[[nodiscard]] float GetAxisValue(Name name) const noexcept;
		[[nodiscard]] static float GetAxisValue(const FInputAxis& axis) noexcept;
		[[nodiscard]] auto& GetEvents() const noexcept { return events_; }
		[[nodiscard]] auto& GetAxises() const noexcept { return axises_; }
		[[nodiscard]] auto& GetActions() const noexcept { return actions_; }

	private:
		std::vector<FInputEvent> events_;
		std::unordered_map<Name, std::vector<FInputAxis>> axises_;
		std::unordered_map<Name, std::vector<FInputAction>> actions_;
	};
}
