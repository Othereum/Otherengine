#pragma once
#include "Engine/GameInstance.hpp"
#include "Input/InputSystem.hpp"

namespace logcat
{
extern ENGINE_API const LogCategory kEngine;
}

namespace oeng
{
inline namespace engine
{
class IRenderer;

class ENGINE_API Engine
{
public:
    DELETE_CPMV(Engine);

    Engine();
    ~Engine();

    void RunLoop();
    void Shutdown();

    void SetRenderer(std::unique_ptr<IRenderer> renderer);
    
    [[nodiscard]] IRenderer& GetRenderer() const noexcept
    {
        return *renderer_;
    }

    InputSystem input_system;
    GameInstance game_instance;

private:
    void Tick();
    void ProcessEvent();

    std::unique_ptr<IRenderer> renderer_;
    size_t ticks_ = 0;
    bool is_running_ = true;
};
}
}
