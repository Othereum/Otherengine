#pragma once
#include "AssetManager.hpp"
#include "InputSystem.hpp"
#include "Audio/AudioSystem.hpp"
#include "Engine/GameInstance.hpp"

namespace oeng
{
inline namespace engine
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
    [[nodiscard]] static Engine& Get() noexcept;

    Engine();

    void RunLoop();
    void Shutdown();

    [[nodiscard]] GameInstance& GetGameInstance() noexcept
    {
        return game_instance_;
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
    InputSystem input_system_;
    GameInstance game_instance_;
    bool is_running_ = true;
};
}
}
