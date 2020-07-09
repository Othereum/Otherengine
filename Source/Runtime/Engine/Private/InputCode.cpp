#include "InputCode.hpp"
#include <SDL2/SDL.h>
#include "Templates/HashMap.hpp"

namespace oeng
{
	String GetName(Keycode btn) noexcept
	{
		return reinterpret_cast<const char8_t*>(SDL_GetKeyName(SDL_Keycode(btn)));
	}

	std::u8string_view GetName(CtrlAxis axis) noexcept
	{
		const auto s = SDL_GameControllerGetStringForAxis(SDL_GameControllerAxis(axis));
		if (s) return reinterpret_cast<const char8_t*>(s);
		return {};
	}

	std::u8string_view GetName(CtrlBtn btn) noexcept
	{
		const auto s = SDL_GameControllerGetStringForButton(SDL_GameControllerButton(btn));
		if (s) return reinterpret_cast<const char8_t*>(s);
		return {};
	}

	DyArr<std::u8string_view> GetNames(KeyMod mod) noexcept
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
			names.push_back(GetName(m));
		
		return names;
	}


	Keycode ToKeycode(std::u8string_view name) noexcept
	{
		const auto str = reinterpret_cast<const char*>(name.data());
		return Keycode(SDL_GetKeyFromName(str));
	}

	CtrlAxis ToCtrlAxis(std::u8string_view name) noexcept
	{
		const auto str = reinterpret_cast<const char*>(name.data());
		return CtrlAxis(SDL_GameControllerGetAxisFromString(str));
	}

	CtrlBtn ToCtrlBtn(std::u8string_view name) noexcept
	{
		const auto str = reinterpret_cast<const char*>(name.data());
		return CtrlBtn(SDL_GameControllerGetButtonFromString(str));
	}

	template <class T>
	static constexpr std::pair<std::u8string_view, T> ToPair(T code) noexcept
	{
		return {GetName(code), code};
	}

	MouseAxis ToMouseAxis(std::u8string_view name) noexcept
	{
		static const HashMap<std::u8string_view, MouseAxis> map
		{
			ToPair(MouseAxis::X),
			ToPair(MouseAxis::Y)
		};
		if (const auto it = map.find(name); it != map.end()) return it->second;
		return MouseAxis::INVALID;
	}

	MouseBtn ToMouseBtn(std::u8string_view name) noexcept
	{
		static const HashMap<std::u8string_view, MouseBtn> map
		{
			ToPair(MouseBtn::L),
			ToPair(MouseBtn::M),
			ToPair(MouseBtn::R),
			ToPair(MouseBtn::X1),
			ToPair(MouseBtn::X2)
		};
		if (const auto it = map.find(name); it != map.end()) return it->second;
		return MouseBtn::INVALID;
	}

	KeyMod ToKeyMod(std::u8string_view name) noexcept
	{
		static const HashMap<std::u8string_view, KeyMod> map
		{
			ToPair(KeyMod::L_SHIFT),
			ToPair(KeyMod::R_SHIFT),
			ToPair(KeyMod::L_CTRL),
			ToPair(KeyMod::R_CTRL),
			ToPair(KeyMod::L_ALT),
			ToPair(KeyMod::R_ALT),
			ToPair(KeyMod::L_GUI),
			ToPair(KeyMod::R_GUI),
			ToPair(KeyMod::NUM),
			ToPair(KeyMod::CAPS),
			ToPair(KeyMod::MODE)
		};
		if (const auto it = map.find(name); it != map.end()) return it->second;
		return KeyMod::NONE;
	}
}
