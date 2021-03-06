﻿#pragma once
#include "Engine/GameInstance.hpp"
#include "Input/InputSystem.hpp"

namespace logcat
{
extern ENGINE_API const LogCategory kEngine;
}

namespace oeng
{
inline namespace rhi
{
class RHIWindow;
}

inline namespace engine
{
class IRenderer;

class ENGINE_API SDLInitializer
{
  public:
    SDLInitializer();
    ~SDLInitializer();
};

class ENGINE_API Engine
{
  public:
    Engine();
    ~Engine();

    void RunLoop();
    void Shutdown();

    void SetRenderer(IRenderer& renderer);

    [[nodiscard]] IRenderer& GetRenderer() const noexcept
    {
        return *renderer_;
    }

    [[nodiscard]] RHIWindow& GetWindow() noexcept
    {
        return *window_;
    }

    InputSystem input_system;
    GameInstance game_instance{*this};

  private:
    void Tick();
    void ProcessEvent();

    std::unique_ptr<RHIWindow> window_;
    IRenderer* renderer_ = nullptr;
    size_t ticks_ = 0;
    bool is_running_ = true;
};
} // namespace engine
} // namespace oeng
