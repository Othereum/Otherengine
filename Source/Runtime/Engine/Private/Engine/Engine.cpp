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
Engine::Engine() : game_instance{*this}
{
    const auto sdl_result = SDL_Init(SDL_INIT_EVERYTHING);
    if (sdl_result != 0)
        throw std::runtime_error{SDL_GetError()};
}

Engine::~Engine()
{
    renderer_.reset();
    SDL_Quit();
}

void Engine::RunLoop()
{
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
}
}
