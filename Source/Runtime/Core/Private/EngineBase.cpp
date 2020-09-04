#include "EngineBase.hpp"
#include <csignal>

namespace logcat
{
const LogCategory kEngine{u8"Engine"sv};
}

namespace oeng
{
inline namespace core
{
static EngineBase* engine_base = nullptr;

static void OnIllegal(int)
{
    OE_LOG(kEngine, kCritical, u8"ILLEGAL INSTRUCTION: It's may be because current CPU is not supported."sv);
}

static void CheckCpu()
{
    const auto& cpu = CpuInfo::Get();
    OE_LOG(kEngine, kLog, cpu.GetBrand());

#ifdef OE_USE_AVX2
    if (!cpu.AVX2())
        throw std::runtime_error{"Unsupported CPU (AVX2)"};
#endif
}

RegisterEngineBase::RegisterEngineBase()
{
    std::signal(SIGILL, &OnIllegal);
    if (IsDebugging())
        OE_LOG(kEngine, kDebug, u8"Debugger detected"sv);
    CheckCpu();

    assert(!engine_base);
    engine_base = reinterpret_cast<EngineBase*>(this);
}

RegisterEngineBase::~RegisterEngineBase()
{
    engine_base = nullptr;
}

EngineBase& EngineBase::Get() noexcept
{
    assert(engine_base);
    return *engine_base;
}

bool EngineBase::Exists() noexcept
{
    return engine_base;
}

EngineBase::EngineBase(std::u8string game_module_path)
    : ticks_{},
      game_dll_{std::move(game_module_path)},
      game_name_{game_dll_.GetSymbol<const char8_t* const>(u8"kGameName"sv)}
{
}
}
}
