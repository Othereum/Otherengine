#pragma once
#include <cstdint>
#include <compare>
#include <typeindex>

namespace Game
{
	enum class EInputType : uint8_t
	{
		keyboard, mButton, mAxis, cButton, cAxis
	};

	enum class EAxisKey
	{
		x, y
	};

	struct FKeyData
	{
		int key;
		uint16_t mod;
		EInputType type;
		auto operator<=>(const FKeyData&) const noexcept = default;
	};

	struct FInputEvent
	{
		FKeyData key;
		bool bPressed;
		auto operator<=>(const FInputEvent&) const noexcept = default;
	};
}

template <>
struct std::hash<Game::FKeyData>
{
	size_t operator()(const Game::FKeyData& key) const noexcept;
};

template <>
struct std::hash<Game::FInputEvent>
{
	size_t operator()(const Game::FInputEvent& key) const noexcept;
};
