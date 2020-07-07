#ifdef OE_MODULE_USE_AVX2
#error Launch module should not use AVX2
#endif

#include <csignal>
#include "Engine.hpp"
#include "Log.hpp"
#include "Platform.hpp"

namespace oeng
{
	static void OnIllegal(int)
	{
		log::Critical(u8"ILLEGAL INSTRUCTION: It's may be because current CPU is not supported.");
	}
	
	static void CheckCpu()
	{
		const auto& cpu = plf::CpuInfo::Get();
		log::Info(u8"CPU: {}", cpu.GetBrand());
#ifdef OE_USE_AVX2
		if (!cpu.AVX2()) throw std::runtime_error{"Unsupported CPU (AVX2)"};
#endif
	}

	OE_IMPORT void SetGameName(std::u8string_view name) noexcept;
	
	static void EngineMain(bool debug)
	{
		std::signal(SIGILL, &OnIllegal);
		
		plf::Dll game_module{u8"./" U8_TEXT(OE_GAME_MODULE)};
		SetGameName(game_module.GetSymbol<std::u8string_view>(u8"kGameName"));

		if (debug) log::Debug(u8"Debugger detected");
		CheckCpu();
		
		auto& load_game = game_module.GetSymbol<void(Engine&)>(u8"LoadGame");
		Engine engine{&load_game};
		engine.RunLoop();
	}
}

int main()  // NOLINT(bugprone-exception-escape)
{
	using namespace oeng;

	if (plf::IsDebugging())
	{
		EngineMain(true);
	}
	else
	{
		try
		{
			EngineMain(false);
		}
		catch (const std::exception& e)
		{
			log::Critical(What(e));
		}
	}
}
