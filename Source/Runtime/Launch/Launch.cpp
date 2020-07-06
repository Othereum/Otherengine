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
		log::Critical("ILLEGAL INSTRUCTION: It's may be because current CPU is not supported.");
	}
	
	static void CheckCpu()
	{
		const auto& cpu = plf::CpuInfo::Get();
		log::Info("CPU: {}", cpu.GetBrand());
#ifdef OE_USE_AVX2
		if (!cpu.AVX2()) throw std::runtime_error{"Unsupported CPU (AVX2)"};
#endif
	}

	static void RunEngine()
	{
		plf::Dll game_module{"./" OE_GAME_MODULE};
		auto& load_game = game_module.GetSymbol<void(Engine&)>("LoadGame");
		const auto game_name = game_module.GetSymbol<std::string_view>("kGameName");
		
		Engine engine{game_name, &load_game};
		engine.RunLoop();
	}

	static void EngineMain()
	{
		std::signal(SIGILL, &OnIllegal);
		CheckCpu();
		RunEngine();
	}
}

int main()  // NOLINT(bugprone-exception-escape)
{
	using namespace oeng;

	if (plf::IsDebugging())
	{
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
			log::Critical(e.what());
		}
	}
}
