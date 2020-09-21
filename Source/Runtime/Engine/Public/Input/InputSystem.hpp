#pragma once
#include "InputCode.hpp"
#include <variant>

struct _SDL_GameController;
union SDL_Event;

namespace logcat
{
extern ENGINE_API const LogCategory kInput;
}

namespace oeng
{
inline namespace engine
{
using InputCode = std::variant<Keycode, MouseBtn, CtrlBtn, MouseAxis, CtrlAxis>;
using CtrlPtr = std::unique_ptr<_SDL_GameController, void (*)(_SDL_GameController*)>;

struct ENGINE_API InputAxis
{
    template <std::convertible_to<InputCode> T>
    constexpr InputAxis(T code, Float scale = 1) noexcept : code{code}, scale{scale}
    {
    }

    explicit InputAxis(const Json& json);

    InputCode code;
    Float scale = 1;
};

struct ENGINE_API InputAction
{
    template <std::convertible_to<InputCode> T>
    constexpr InputAction(T code, KeyMod mod = KeyMod::NONE) noexcept : code{code}, mod{mod}
    {
    }

    explicit InputAction(const Json& json);

    InputCode code;
    KeyMod mod;
};

struct AxisConfig
{
    Float dead_zone = 0_f;
    Float sensitivity = 1_f;
};

struct ENGINE_API ParsedEvent
{
    ParsedEvent(InputCode code, bool pressed);
    InputAction input;
    bool pressed;
};

ENGINE_API std::string ToString(InputCode code);
ENGINE_API InputCode ToInputCode(std::string_view str);
ENGINE_API void to_json(Json& json, const InputAxis& axis);
ENGINE_API void to_json(Json& json, const InputAction& action);
ENGINE_API void to_json(Json& json, const AxisConfig& action);
ENGINE_API void from_json(const Json& json, AxisConfig& action);

class ENGINE_API InputSystem
{
  public:
    DELETE_CPMV(InputSystem);

    void SaveConfig();

    [[nodiscard]] Float GetAxisValue(Name name) const;
    [[nodiscard]] Float GetAxisValue(InputAxis axis) const;
    [[nodiscard]] Float GetAxisValue(CtrlBtn code) const noexcept;
    [[nodiscard]] Float GetAxisValue(CtrlAxis code) const noexcept;
    [[nodiscard]] Float GetAxisValue(MouseAxis code) const noexcept;
    [[nodiscard]] Float GetAxisValue(MouseBtn code) const noexcept;
    [[nodiscard]] Float GetAxisValue(Keycode code) const noexcept;

    [[nodiscard]] auto& GetEvents() const noexcept
    {
        return events_;
    }

    std::unordered_map<Name, std::vector<InputAxis>> axises;
    std::unordered_map<Name, std::vector<InputAction>> actions;
    std::unordered_map<InputCode, AxisConfig> axis_configs;

  private:
    friend class Engine;
    InputSystem();

    void AddEvent(const SDL_Event& e);
    void AddEvent(ParsedEvent e);

    void ClearEvents()
    {
        events_.clear();
    }

    void PostAddAllEvents();
    void AddController(int id);
    void RemoveController(int id);

    [[nodiscard]] Float FilterAxis(InputCode code, Float val) const noexcept;
    [[nodiscard]] Vec2 FilterAxis(InputCode code, Vec2 val) const noexcept;
    [[nodiscard]] _SDL_GameController* Ctrl() const noexcept;

    struct InputEvent
    {
        Name name;
        bool pressed = false;
    };

    std::vector<InputEvent> events_;
    std::vector<CtrlPtr> controllers_;

    Vec2 mouse_;

    struct
    {
        bool l, r;
    } trig_{};
};
} // namespace engine
} // namespace oeng
