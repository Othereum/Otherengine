#include "Engine/Engine.hpp"
#include "DynamicRHI.hpp"
#include "IRenderer.hpp"
#include "Input/InputSystem.hpp"
#include "RHIWindow.hpp"
#include <SDL2/SDL.h>

namespace logcat
{
const LogCategory kEngine{u8"Engine"sv};
}

namespace oeng
{
inline namespace engine
{
SDLInitializer::SDLInitializer()
{
    const auto sdl_result = SDL_Init(SDL_INIT_EVERYTHING);
    if (sdl_result != 0)
        throw std::runtime_error{SDL_GetError()};
}

SDLInitializer::~SDLInitializer()
{
    SDL_Quit();
}

[[nodiscard]] static auto InitDisplay()
{
    const auto num_dp = SDL_GetNumVideoDisplays();
    if (num_dp < 1)
        throw std::runtime_error{SDL_GetError()};

    auto& cfgsys = ConfigSystem::Get();
    const Name display = u8"Display"sv;

    // List supported display modes
    auto& cfg = cfgsys(display);
    auto& displays = cfg["Display modes (read only)"s] = Json::object();
    for (auto dp = 0; dp < num_dp; ++dp)
    {
        const auto num_dm = SDL_GetNumDisplayModes(dp);
        if (num_dm < 1)
            throw std::runtime_error{SDL_GetError()};

        auto& modes = displays[fmt::format(FMT_COMPILE("Display {}"sv), dp)] = Json::array();
        for (auto dm = 0; dm < num_dm; ++dm)
        {
            SDL_DisplayMode mode;
            SDL_GetDisplayMode(dp, dm, &mode);

            auto str = fmt::format(u8"[{}] {}x{} {}Hz"sv, dm, mode.w, mode.h, mode.refresh_rate);
            modes.emplace_back(AsString(std::move(str)));
        }
    }

    std::tuple<int, SDL_DisplayMode, bool, SwapInterval> ret;
    auto& [dp_idx, dp_mode, fullscreen, swap_interval] = ret;

    fullscreen = cfg.at("Fullscreen"s).get<bool>();

    swap_interval = cfg.at("VSync"s).get<bool>()
                        ? cfg.at("VSync_Adaptive").get<bool>() ? SwapInterval::adaptive : SwapInterval::synchronized
                        : SwapInterval::immediate;

    auto& dp_idx_cfg = cfg.at("Display"s);
    dp_idx = dp_idx_cfg.get<int>();

    // Check if display is valid
    if (dp_idx >= num_dp)
    {
        OE_LOG(kEngine, kWarn, u8"Attempted to use a non-existent display (tried: {}, max: {})"sv, dp_idx, num_dp - 1);
        OE_LOG(kEngine, kWarn, u8"Using display 0..."sv);
        dp_idx_cfg = dp_idx = 0;
    }

    const auto num_dm = SDL_GetNumDisplayModes(dp_idx);
    auto dm_idx_cfg = cfg.at("FullscreenDisplayMode"s);
    auto dm_idx = dm_idx_cfg.get<int>();

    // Check if display mode is valid
    if (dm_idx >= num_dm)
    {
        OE_LOG(kEngine, kWarn, u8"Attempted to use a non-existent display mode (tried: {}, max: {})"sv, dm_idx,
               num_dm - 1);
        OE_LOG(kEngine, kWarn, u8"Using display mode 0..."sv);
        dm_idx_cfg = dm_idx = 0;
    }

    // Initialize windowed size
    auto ws = "WindowedSize"s;
    auto ws_cfg = cfg.find(ws);
    if (ws_cfg == cfg.end())
    {
        SDL_DisplayMode wm;
        SDL_GetDesktopDisplayMode(dp_idx, &wm);

        Vector size{wm.w, wm.h};
        size *= 5, size /= 6;
        ws_cfg = cfg.emplace(std::move(ws), size).first;
    }

    // Fill SDL_DisplayMode
    if (fullscreen)
    {
        if (0 != SDL_GetDisplayMode(dp_idx, dm_idx, &dp_mode))
            throw std::runtime_error{SDL_GetError()};
    }
    else
    {
        const auto size = ws_cfg->get<Vec2i>();
        dp_mode.w = size[0];
        dp_mode.h = size[1];
    }

    cfgsys.Save(display);
    return ret;
}

Engine::Engine()
{
    auto [dp_idx, dp_mode, fullscreen, swap_interval] = InitDisplay();

    window_.reset(DynamicRHI::Get().CreateWindow(
        fmt::format(u8"{} ({}x{} {}Hz)"sv, GetGameName(), dp_mode.w, dp_mode.h, dp_mode.refresh_rate).c_str(),
        SDL_WINDOWPOS_UNDEFINED_DISPLAY(dp_idx), SDL_WINDOWPOS_UNDEFINED_DISPLAY(dp_idx), dp_mode.w, dp_mode.h,
        fullscreen ? SDL_WINDOW_FULLSCREEN : 0));

    window_->SetRefreshRate(dp_mode.refresh_rate);
    window_->SetSwapInterval(swap_interval);
}

void Engine::RunLoop()
{
    assert(renderer_);
    game_instance.world.BeginPlay();

    OE_LOG(kEngine, kLog, u8"Engine loop started."sv);
    const auto start = Clock::now();

    while (is_running_)
    {
        Tick();
        ++ticks_;
    }

    const auto elapsed = Clock::now() - start;
    const auto sec = duration_cast<time::duration<Float>>(elapsed).count();
    const auto ms = duration_cast<time::duration<Float, std::milli>>(elapsed).count();
    OE_LOG(kEngine, kLog, u8"Average fps: {:.0f}, frame time: {:.2f} ms"sv, ticks_ / sec, ms / ticks_);
}

void Engine::Shutdown()
{
    is_running_ = false;
    OE_LOG(kEngine, kLog, u8"Engine shutdown requested."sv);
}

void Engine::SetRenderer(IRenderer& renderer)
{
    assert(!renderer_);
    renderer_ = &renderer;
}

void Engine::Tick()
{
    ProcessEvent();
    game_instance.world.Tick();
    renderer_->DrawScene(game_instance.local_player.GetViewPoint());
}

void Engine::ProcessEvent()
{
    input_system.ClearEvents();

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
        {
            Shutdown();
            return;
        }

        input_system.AddEvent(event);
    }

    input_system.PostAddAllEvents();
}
} // namespace engine
} // namespace oeng
