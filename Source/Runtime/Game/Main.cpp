#ifdef OE_MODULE_USE_AVX2
#error Game loader module should not use AVX2
#endif

#include "Engine.hpp"
#include "Log.hpp"
#include "Platform.hpp"

namespace oeng
{
	static void CheckCPU()
	{
		const auto& cpu = plf::CpuInfo::Get();
		log::Info("CPU: {}", cpu.GetBrand());
#ifdef OE_USE_AVX2
		if (!cpu.AVX2()) throw std::runtime_error{"Unsupported CPU (AVX2)"};
#endif
	}

	static void EngineMain(int argc, char** argv)
	{
		CheckCPU();

		plf::Dll game_module{argc > 1 ? argv[1] : "./" OE_DEFAULT_GAME_MODULE};
		auto& load_game = game_module.GetSymbol<void(Engine&)>("LoadGame");
		const auto game_name = game_module.GetSymbol<const char*>("kGameName");
		
		Engine{game_name, &load_game}.RunLoop();
	}
}

int main(int argc, char** argv)  // NOLINT(bugprone-exception-escape)
{
	using namespace oeng;

	if (plf::IsDebugging())
	{
		EngineMain(argc, argv);
	}
	else
	{
		try
		{
			EngineMain(argc, argv);
		}
		catch (const std::exception& e)
		{
			log::Critical(e.what());
		}
	}
}
