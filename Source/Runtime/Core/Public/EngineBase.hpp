#pragma once
#include "ConfigSystem.hpp"
#include "Log.hpp"
#include "Platform.hpp"
#include "Stat.hpp"

namespace logcat
{
extern CORE_API const LogCategory kEngine;
}

namespace oeng
{
inline namespace engine
{
class Engine;
}

inline namespace core
{
class EngineBase;

class CORE_API RegisterEngineBase
{
    friend EngineBase;
    DELETE_CPMV(RegisterEngineBase);
    RegisterEngineBase();
    ~RegisterEngineBase();
};

class CORE_API EngineBase : RegisterEngineBase
{
public:
    DELETE_CPMV(EngineBase);

    [[nodiscard]] static EngineBase& Get() noexcept;
    [[nodiscard]] static bool Exists() noexcept;

    [[nodiscard]] auto GetTickCount() const noexcept
    {
        return ticks_;
    }

    [[nodiscard]] auto GetGameName() const noexcept
    {
        return game_name_;
    }

    [[nodiscard]] auto& GetGameDll() const noexcept
    {
        return game_dll_;
    }

    [[nodiscard]] auto& GetLogger() noexcept
    {
        return logger_;
    }

    [[nodiscard]] auto& GetConfigSystem() noexcept
    {
        return config_system_;
    }

protected:
    uint64_t ticks_;

private:
    friend Engine;
    friend CounterManager;
    friend ScopeCounter;
    friend ScopeStackCounter;

    explicit EngineBase(std::u8string game_module_path);
    ~EngineBase() = default;

    Dll game_dll_;
    std::u8string_view game_name_;
    Logger logger_;
    ConfigSystem config_system_;
    CounterManager counters_;
};
}
}
