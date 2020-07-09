#include "InputCode.hpp"
#include <SDL2/SDL.h>
#include "Templates/HashMap.hpp"

namespace oeng
{
	using namespace std::literals;

	std::u8string GetName(Keycode btn) noexcept
	{
		return reinterpret_cast<const char8_t*>(SDL_GetKeyName(SDL_Keycode(btn)));
	}

	std::u8string_view GetName(ConAxis axis) noexcept
	{
		const auto s = SDL_GameControllerGetStringForAxis(SDL_GameControllerAxis(axis));
		if (s) return reinterpret_cast<const char8_t*>(s);
		return {};
	}

	std::u8string_view GetName(ConBtn btn) noexcept
	{
		const auto s = SDL_GameControllerGetStringForButton(SDL_GameControllerButton(btn));
		if (s) return reinterpret_cast<const char8_t*>(s);
		return {};
	}

	std::u8string_view GetName(MouseAxis axis) noexcept
	{
		switch (axis)
		{
		case MouseAxis::X: return u8"X"sv;
		case MouseAxis::Y: return u8"Y"sv;
		default: return {};
		}
	}

	std::u8string_view GetName(MouseBtn btn) noexcept
	{
		switch (btn)
		{
		case MouseBtn::L: return u8"L"sv;
		case MouseBtn::M: return u8"M"sv;
		case MouseBtn::R: return u8"R"sv;
		case MouseBtn::X1: return u8"X1"sv;
		case MouseBtn::X2: return u8"X2"sv;
		default: return {};
		}
	}

	static std::u8string_view GetSingleName(KeyMod mod) noexcept
	{
		switch (mod)
		{
		case KeyMod::L_SHIFT: return u8"Left Shift"sv;
		case KeyMod::R_SHIFT: return u8"Right Shift"sv;
		case KeyMod::L_CTRL: return u8"Left Ctrl"sv;
		case KeyMod::R_CTRL: return u8"Right Ctrl"sv;
		case KeyMod::L_ALT: return u8"Left Alt"sv;
		case KeyMod::R_ALT: return u8"Right Alt"sv;
		case KeyMod::L_GUI: return u8"Left GUI"sv;
		case KeyMod::R_GUI: return u8"Right GUI"sv;
		case KeyMod::NUM: return u8"Num Lock"sv;
		case KeyMod::CAPS: return u8"Caps Lock"sv;
		case KeyMod::MODE: return u8"AltGr"sv;
		default: assert(false); return {};
		}
	}

	DyArr<std::u8string_view> GetName(KeyMod mod) noexcept
	{
		static constexpr KeyMod mods[]
		{
			KeyMod::L_SHIFT, KeyMod::R_SHIFT,
			KeyMod::L_CTRL, KeyMod::R_CTRL,
			KeyMod::L_ALT, KeyMod::R_ALT,
			KeyMod::L_GUI, KeyMod::R_GUI,
			KeyMod::NUM, KeyMod::CAPS, KeyMod::MODE
		};
		
		DyArr<std::u8string_view> names;
		
		for (auto m : mods) if ((mod & m) != KeyMod::NONE)
			names.push_back(GetSingleName(m));
		
		return names;
	}


	std::optional<Keycode> ToKeycode(std::u8string_view name) noexcept
	{
	}

	std::optional<ConAxis> ToConAxis(std::u8string_view name) noexcept
	{
	}

	std::optional<ConBtn> ToConBtn(std::u8string_view name) noexcept
	{
	}

	std::optional<MouseAxis> ToMouseAxis(std::u8string_view name) noexcept
	{
	}

	std::optional<MouseBtn> ToMouseBtn(std::u8string_view name) noexcept
	{
	}

	std::optional<KeyMod> ToKeyMod(std::u8string_view name) noexcept
	{
	}
}
