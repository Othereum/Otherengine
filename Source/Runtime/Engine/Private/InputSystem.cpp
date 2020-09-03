#include <SDL_events.h>
#include "Debug.hpp"
#include "Engine.hpp"
#include "Math.hpp"

namespace logcat
{
const LogCategory kInput{u8"Input"sv};
}

namespace oeng
{
inline namespace engine
{
template <class... Ts>
struct Overload : Ts...
{
    using Ts::operator()...;
};

template <class... Ts>
Overload(Ts ...) -> Overload<Ts...>;

static KeyMod GetModState() noexcept
{
    auto mod = static_cast<int>(SDL_GetModState());

    if (mod & KMOD_SHIFT)
        mod |= KMOD_SHIFT;

    if (mod & KMOD_CTRL)
        mod |= KMOD_CTRL;

    if (mod & KMOD_ALT)
        mod |= KMOD_ALT;

    if (mod & KMOD_GUI)
        mod |= KMOD_GUI;

    return KeyMod(mod);
}

static std::optional<ParsedEvent> ParseEvent(const SDL_Event& e)
{
    switch (e.type)
    {
    case SDL_KEYDOWN: case SDL_KEYUP:
        if (e.key.repeat)
            return std::nullopt;
        return ParsedEvent(Keycode(e.key.keysym.sym), !!e.key.state);

    case SDL_MOUSEBUTTONDOWN: case SDL_MOUSEBUTTONUP:
        return ParsedEvent(MouseBtn(e.button.button), !!e.button.state);

    case SDL_CONTROLLERBUTTONDOWN: case SDL_CONTROLLERBUTTONUP:
        return ParsedEvent(CtrlBtn(e.cbutton.button), !!e.cbutton.state);

    default:
        return std::nullopt;
    }
}

static bool IsMatch(const InputAction& event, const std::vector<InputAction>& keys)
{
    for (const auto& key : keys)
        if (event.code == key.code && (event.mod & key.mod) == key.mod)
            return true;
    return false;
}

InputCode ToInputCode(std::string_view str)
{
    static const std::unordered_map<std::string_view, std::function<std::optional<InputCode>(std::u8string_view)>>
        parsers
        {
            {"Keycode"sv, ToKeycode},
            {"MouseBtn"sv, ToMouseBtn},
            {"CtrlBtn"sv, ToCtrlBtn},
            {"MouseAxis"sv, ToMouseAxis},
            {"CtrlAxis"sv, ToCtrlAxis},
        };

    const auto del = str.find('.');
    if (del == std::u8string_view::npos)
        throw std::invalid_argument{"Format should be like '<type>.<code>'"};

    const auto type = str.substr(0, del);
    const auto code = str.substr(del + 1);

    try
    {
        return parsers.at(type)(AsString8(code)).value();
    }
    catch (const std::out_of_range&)
    {
        throw std::invalid_argument{fmt::format(
            "Invalid type '{}' (Keycode, MouseBtn, CtrlBtn, MouseAxis, CtrlAxis)", type
            )};
    }
    catch (const std::bad_optional_access&)
    {
        throw std::invalid_argument{fmt::format("Invalid code '{}'", code)};
    }
}

std::string ToString(InputCode code)
{
    const auto type = std::visit(
        Overload{
            [](Keycode)
            {
                return u8"Keycode"sv;
            },
            [](MouseBtn)
            {
                return u8"MouseBtn"sv;
            },
            [](CtrlBtn)
            {
                return u8"CtrlBtn"sv;
            },
            [](MouseAxis)
            {
                return u8"MouseAxis"sv;
            },
            [](CtrlAxis)
            {
                return u8"CtrlAxis"sv;
            }
        }, code);

    std::u8string buffer;
    const auto name = std::visit(
        Overload{
            [&](Keycode c)-> std::u8string_view
            {
                return buffer = GetName(c);
            },
            [](auto c)
            {
                return GetName(c);
            }
        }, code);

    return AsString(fmt::format(u8"{}.{}"sv, type, name));
}

void to_json(Json& json, const InputAxis& axis)
{
    json["Code"] = ToString(axis.code);
    json["Scale"] = axis.scale;
}

void to_json(Json& json, const InputAction& action)
{
    json["Code"] = ToString(action.code);

    if (auto mods = GetNames(action.mod); !mods.empty())
    {
        auto& out = json["Mods"] = Json::array();
        for (const auto mod : mods)
            out.emplace_back(AsString(mod));
    }
}

void to_json(Json& json, const AxisConfig& action)
{
    json["DeadZone"] = action.dead_zone;
    json["Sensitivity"] = action.sensitivity;
}

void from_json(const Json& json, AxisConfig& action)
{
    action.dead_zone = json.at("DeadZone").get<Float>();
    action.sensitivity = json.at("Sensitivity").get<Float>();
}

template <class Map>
static void LoadInput(const Json& config, const char* key, Map& mapped)
{
    const auto map = config.find(key);
    if (map == config.end())
        return;

    for (auto& [name, inputs] : map->items())
    {
        auto& arr = mapped[std::u8string{AsString8(name)}];
        const auto size = inputs.size();
        for (size_t i = 0; i < size; ++i)
            try
            {
                arr.emplace_back(inputs[i]);
            }
            catch (const std::exception& e)
            {
                OE_LOG(kInput, kErr, u8"Failed to load input mapping {}.{}[{}]: {}"sv,
                       AsString8(key), AsString8(name), i, What(e));
            }
    }
}

InputAxis::InputAxis(const Json& json)
    : code{ToInputCode(json.at("Code").get<std::string>())}, scale{json.at("Scale")}
{
}

InputAction::InputAction(const Json& json)
    : code{ToInputCode(json.at("Code").get<std::string>())}, mod{KeyMod::NONE}
{
    if (const auto mods_in = json.find("Mods"); mods_in != json.end())
    {
        for (const auto& mod_in : mods_in.value())
            try
            {
                mod |= ToKeyMod(AsString8(mod_in.get<std::string>())).value();
            }
            catch (const std::bad_optional_access&)
            {
                throw std::runtime_error{fmt::format("Invalid mod '{}'"sv, mod_in.get<std::string>())};
            }
    }
}

InputSystem& InputSystem::Get() noexcept
{
    return Engine::Get().GetInputSystem();
}

InputSystem::InputSystem()
{
    if (0 != SDL_SetRelativeMouseMode(SDL_TRUE))
        OE_LOG(kInput, kErr, AsString8(SDL_GetError()));

    const auto& config = ConfigSystem::Get()(u8"Input"sv);
    LoadInput(config, "ActionMap", actions);
    LoadInput(config, "AxisMap", axises);

    if (const auto axis_cfg = config.find("AxisConfig"); axis_cfg != config.end())
        for (auto& [code, cfg] : axis_cfg->items())
            axis_configs.try_emplace(ToInputCode(code), cfg);
}

void InputSystem::AddEvent(const SDL_Event& e)
{
    switch (e.type)
    {
    case SDL_CONTROLLERDEVICEADDED:
        AddController(e.cdevice.which);
        break;

    case SDL_CONTROLLERDEVICEREMOVED:
        RemoveController(e.cdevice.which);
        break;

    default:
        if (auto parsed = ParseEvent(e))
            AddEvent(*parsed);
    }
}

void InputSystem::PostAddAllEvents()
{
    int x, y;
    SDL_GetRelativeMouseState(&x, &y);
    mouse_[0] = static_cast<float>(x);
    mouse_[1] = static_cast<float>(y);

    auto trigger = [&](bool& old, CtrlAxis axis)
    {
        const auto pressed = GetAxisValue(axis) > 0.25_f;
        if (old != pressed)
            AddEvent({axis, pressed});
        old = pressed;
    };
    trigger(trig_.l, CtrlAxis::LT);
    trigger(trig_.r, CtrlAxis::RT);
}

Float InputSystem::GetAxisValue(Name name) const
{
    auto it = axises.find(name);
    if (it == axises.end())
        return 0;

    auto val = 0_f;
    for (const auto& axis : it->second)
    {
        val += GetAxisValue(axis);
    }
    return val;
}

Float InputSystem::GetAxisValue(InputAxis axis) const
{
    auto get = [&](auto c)
    {
        return GetAxisValue(c);
    };
    const auto val = std::visit(get, axis.code);
    return val * axis.scale;
}

Float InputSystem::GetAxisValue(CtrlBtn code) const noexcept
{
    const auto btn = SDL_GameControllerButton(code);
    const auto val = SDL_GameControllerGetButton(Ctrl(), btn) ? 1_f : 0_f;
    return FilterAxis(code, val);
}

Float InputSystem::GetAxisValue(CtrlAxis code) const noexcept
{
    auto raw = [&](CtrlAxis c)
    {
        const auto axis = SDL_GameControllerAxis(c);
        const auto val = SDL_GameControllerGetAxis(Ctrl(), axis);
        return ToFloat(val) / (val >= 0 ? 32767_f : 32768_f);
    };

    auto stick = [&](CtrlAxis c)
    {
        const auto is_y = static_cast<int>(c) % 2;
        const auto x_axis = static_cast<int>(c) / 2 * 2;
        return FilterAxis(c, {raw(CtrlAxis(x_axis)), raw(CtrlAxis(x_axis + 1))})[is_y];
    };

    switch (code)
    {
    case CtrlAxis::LX: case CtrlAxis::LY: case CtrlAxis::RX: case CtrlAxis::RY:
        return stick(code);

    default:
        return FilterAxis(code, raw(code));
    }
}

Float InputSystem::GetAxisValue(MouseAxis code) const noexcept
{
    Float val;

    switch (code)
    {
    case MouseAxis::X:
        val = mouse_[0];
        break;
    case MouseAxis::Y:
        val = mouse_[1];
        break;
    default:
        val = 0_f;
    }

    return FilterAxis(code, val);
}

Float InputSystem::GetAxisValue(MouseBtn code) const noexcept
{
    const auto btn = SDL_GetMouseState(nullptr, nullptr);
    const auto val = btn & MouseMask(code) ? 1_f : 0_f;
    return FilterAxis(code, val);
}

Float InputSystem::GetAxisValue(Keycode code) const noexcept
{
    const auto scan = SDL_GetScancodeFromKey(SDL_Keycode(code));
    const auto val = SDL_GetKeyboardState(nullptr)[scan] ? 1_f : 0_f;
    return FilterAxis(code, val);
}

void InputSystem::AddController(int id)
{
    if (auto* const ctrl = SDL_GameControllerOpen(id))
    {
        controllers_.emplace_back(ctrl, &SDL_GameControllerClose);
    }
    else
    {
        OE_LOG(kInput, kErr, u8"Could not open game controller {}: {}"sv, id,
               reinterpret_cast<const char8_t*>(SDL_GetError()));
    }
}

void InputSystem::RemoveController(int id)
{
    const auto found = std::find_if(controllers_.begin(), controllers_.end(), [&](const CtrlPtr& p)
    {
        return id == SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(p.get()));
    });

    if (ENSURE(found != controllers_.end()))
    {
        controllers_.erase(found);
    }
}

Float InputSystem::FilterAxis(InputCode code, Float val) const noexcept
{
    const auto config = axis_configs.find(code);
    if (config != axis_configs.end())
    {
        auto& cfg = config->second;

        val = val >= 0
                  ? Max(0, MapRng({cfg.dead_zone, 1}, {0, 1}, val))
                  : Min(0, MapRng({-1, -cfg.dead_zone}, {-1, 0}, val));

        val *= cfg.sensitivity;
    }
    return val;
}

Vec2 InputSystem::FilterAxis(InputCode code, Vec2 val) const noexcept
{
    const auto config = axis_configs.find(code);
    if (config != axis_configs.end())
    {
        auto& cfg = config->second;
        const auto len = val.Len();

        if (len <= Max(cfg.dead_zone, kSmallNum))
        {
            val = {};
        }
        else
        {
            const auto new_len = MapRng({cfg.dead_zone, 1}, {0, 1}, len);
            val *= new_len * cfg.sensitivity / len;
        }
    }
    return val;
}

ParsedEvent::ParsedEvent(InputCode code, bool pressed)
    : input{code, GetModState()}, pressed{pressed}
{
}

void InputSystem::AddEvent(ParsedEvent e)
{
    for (const auto& act : actions)
        if (IsMatch(e.input, act.second))
            events_.push_back({act.first, e.pressed});
}

void InputSystem::SaveConfig()
{
    auto& configs = ConfigSystem::Get();
    const Name conf_name = u8"Input"sv;
    auto& config = configs(conf_name);

    auto save = [&](const char* key, const auto& mapped)
    {
        auto& map = config[key];
        for (auto& [name, inputs] : mapped)
        {
            auto& out_inputs = map[AsString(*name).c_str()] = Json::array();
            for (auto& input : inputs)
                out_inputs.emplace_back(input);
        }
    };
    save("ActionMap", actions);
    save("AxisMap", axises);

    auto& out_axis_cfg = config["AxisConfig"] = Json::object();
    for (auto& [code, cfg] : axis_configs)
        out_axis_cfg[ToString(code).c_str()] = cfg;

    (void)configs.Save(conf_name);
}

_SDL_GameController* InputSystem::Ctrl() const noexcept
{
    return controllers_.empty() ? nullptr : controllers_.front().get();
}
}
}
