#include "Engine/Engine.hpp"
#include "IRenderer.hpp"
#include "Input/InputSystem.hpp"
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

static void WriteDisplayModes()
{
    const auto num_dp = SDL_GetNumVideoDisplays();
    if (num_dp < 1)
        throw std::runtime_error{SDL_GetError()};

    auto& displays = ConfigSystem::Get()(u8"Display"sv)["Display modes (read only)"s] = Json::object();
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
}

static void WriteWindowSize()
{
    auto& cfg = ConfigSystem::Get()(u8"Display"sv);
    const auto windowed_size = "WindowedSize"s;
    if (!cfg.contains(windowed_size))
    {
        SDL_DisplayMode dp_mode;
        SDL_GetDesktopDisplayMode(cfg.at("Display"s).get<int>(), &dp_mode);

        Vector size{dp_mode.w, dp_mode.h};
        size *= 5, size /= 6;
        cfg[windowed_size] = size;
    }
}

Engine::Engine()
{
    WriteDisplayModes();
    WriteWindowSize();
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
