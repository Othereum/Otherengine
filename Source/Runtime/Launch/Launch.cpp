#ifdef OE_MODULE_USE_AVX2
#error Launch module should not use AVX2
#endif

#include "Engine/Engine.hpp"
#include <csignal>

using namespace oeng;

namespace oeng
{
inline namespace core
{
OE_IMPORT void SetGameName(std::u8string_view game_name);
}
}

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

static void EngineMain()
{
    CheckCpu();

    const Dll game_dll{u8"./" U8_TEXT(OE_GAME_MODULE)};
    SetGameName(game_dll.GetSymbol<const std::u8string_view>(u8"kGameName"sv));
    Finally _{&FlushLog};

    Engine engine;
    engine.RunLoop();
}

int main()
{
    std::signal(SIGILL, &OnIllegal);

    if (IsDebugging())
    {
        OE_LOG(kEngine, kDebug, u8"Debugger detected"sv);
        EngineMain();
    }
    else
    {
        try
        {
            EngineMain();
        }
        catch (const std::exception& e)
        {
            OE_LOG(kEngine, kCritical, AsString8(e.what()));
            return EXIT_FAILURE;
        }
    }
}
