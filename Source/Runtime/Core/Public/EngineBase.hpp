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
    explicit EngineBase(std::u8string game_module_path);

    [[nodiscard]] auto GetTickCount() const noexcept
    {
        return ticks_;
    }

    [[nodiscard]] auto GetGameName() const noexcept
    {
        return game_name_;
    }

    Dll game_dll;
    ConfigSystem config_system;

protected:
    uint64_t ticks_ = 0;

private:
    std::u8string_view game_name_;
};
}
}
