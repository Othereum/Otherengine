#include "Input/InputCode.hpp"
#include <SDL_gamecontroller.h>
#include <SDL_keyboard.h>
#include <unordered_map>

namespace oeng
{
inline namespace engine
{
std::u8string GetName(Keycode btn) noexcept
{
    return AsString8(SDL_GetKeyName(SDL_Keycode(btn)));
}

std::u8string_view GetName(CtrlAxis axis) noexcept
{
    const auto* const s = SDL_GameControllerGetStringForAxis(SDL_GameControllerAxis(axis));
    if (s)
        return AsString8(s);
    return {};
}

std::u8string_view GetName(CtrlBtn btn) noexcept
{
    const auto* const s = SDL_GameControllerGetStringForButton(SDL_GameControllerButton(btn));
    if (s)
        return AsString8(s);
    return {};
}

std::vector<std::u8string_view> GetNames(KeyMod mod) noexcept
{
    static constexpr KeyMod mods[]
    {
        KeyMod::SHIFT, KeyMod::CTRL, KeyMod::ALT
    };

    std::vector<std::u8string_view> names;

    for (auto m : mods)
        if ((mod & m) != KeyMod::NONE)
            names.push_back(GetName(m));

    return names;
}


std::optional<Keycode> ToKeycode(std::u8string_view name) noexcept
{
    const auto* const str = AsString(name.data());
    const auto key = SDL_GetKeyFromName(str);
    if (key != SDLK_UNKNOWN)
        return Keycode(key);
    return std::nullopt;
}

std::optional<CtrlAxis> ToCtrlAxis(std::u8string_view name) noexcept
{
    const auto* const str = AsString(name.data());
    const auto axis = SDL_GameControllerGetAxisFromString(str);
    if (axis != SDL_CONTROLLER_AXIS_INVALID)
        return CtrlAxis(axis);
    return std::nullopt;
}

std::optional<CtrlBtn> ToCtrlBtn(std::u8string_view name) noexcept
{
    const auto* const str = AsString(name.data());
    const auto btn = SDL_GameControllerGetButtonFromString(str);
    if (btn != SDL_CONTROLLER_BUTTON_INVALID)
        return CtrlBtn(btn);
    return std::nullopt;
}

template <class T>
static constexpr std::pair<std::u8string_view, T> ToPair(T code) noexcept
{
    return {GetName(code), code};
}

std::optional<MouseAxis> ToMouseAxis(std::u8string_view name) noexcept
{
    static const std::unordered_map<std::u8string_view, MouseAxis> map
    {
        ToPair(MouseAxis::X),
        ToPair(MouseAxis::Y)
    };
    if (const auto it = map.find(name); it != map.end())
        return it->second;
    return std::nullopt;
}

std::optional<MouseBtn> ToMouseBtn(std::u8string_view name) noexcept
{
    static const std::unordered_map<std::u8string_view, MouseBtn> map
    {
        ToPair(MouseBtn::L),
        ToPair(MouseBtn::M),
        ToPair(MouseBtn::R),
        ToPair(MouseBtn::X1),
        ToPair(MouseBtn::X2)
    };
    if (const auto it = map.find(name); it != map.end())
        return it->second;
    return std::nullopt;
}

std::optional<KeyMod> ToKeyMod(std::u8string_view name) noexcept
{
    static const std::unordered_map<std::u8string_view, KeyMod> map
    {
        ToPair(KeyMod::SHIFT),
        ToPair(KeyMod::CTRL),
        ToPair(KeyMod::ALT),
        ToPair(KeyMod::GUI),
        ToPair(KeyMod::NUM),
        ToPair(KeyMod::CAPS),
        ToPair(KeyMod::MODE)
    };
    if (const auto it = map.find(name); it != map.end())
        return it->second;
    return std::nullopt;
}
}
}
