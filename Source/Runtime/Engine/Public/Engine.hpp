#pragma once
#include "AssetManager.hpp"
#include "EngineBase.hpp"
#include "InputSystem.hpp"
#include "Renderer.hpp"
#include "World.hpp"
#include "Audio/AudioSystem.hpp"

namespace oeng::engine
{
class ENGINE_API InitEngine
{
    DELETE_CPMV(InitEngine);
    explicit InitEngine(Engine* e);
    ~InitEngine();
    friend Engine;
};

class ENGINE_API Engine : public EngineBase, InitEngine
{
public:
    DELETE_CPMV(Engine);

    [[nodiscard]] static Engine& Get() noexcept;

    explicit Engine(std::u8string game_module_path);
    ~Engine() = default;

    void RunLoop();
    void Shutdown();

    [[nodiscard]] World& GetWorld() noexcept
    {
        return world_;
    }

    [[nodiscard]] Renderer& GetRenderer() noexcept
    {
        return renderer_;
    }

    [[nodiscard]] InputSystem& GetInputSystem() noexcept
    {
        return input_system_;
    }

    [[nodiscard]] AssetManager& GetAssetManager() noexcept
    {
        return asset_manager_;
    }

private:
    void Tick();
    void ProcessEvent();

    AssetManager asset_manager_;
    AudioSystem audio_system_;
    Renderer renderer_;
    InputSystem input_system_;
    World world_;
    bool is_running_ = true;
};
}
