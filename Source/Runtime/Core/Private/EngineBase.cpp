#include "EngineBase.hpp"
#include <csignal>

namespace logcat
{
	const LogCategory kEngine{u8"Engine"sv};
}
	
namespace oeng::core
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
		if (!cpu.AVX2()) throw std::runtime_error{"Unsupported CPU (AVX2)"};
#endif
	}
	
	RegisterEngineBase::RegisterEngineBase(EngineBase* engine)
	{
		std::signal(SIGILL, &OnIllegal);
		if (IsDebugging()) OE_LOG(kEngine, kDebug, u8"Debugger detected"sv);
		CheckCpu();
		
		assert(engine);
		assert(!engine_base);
		engine_base = engine;
	}

	RegisterEngineBase::~RegisterEngineBase()
	{
		engine_base = nullptr;
	}

	CoreSystem::CoreSystem(std::u8string game_module_path)
		:game_dll_{std::move(game_module_path)},
		game_name_{game_dll_.GetSymbol<const char8_t* const>(u8"kGameName"sv)}
	{
		InitMemPool();
	}

	CoreSystem::~CoreSystem()
	{
		CleanUpMemPool();
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
		:RegisterEngineBase(this),
		CoreSystem{std::move(game_module_path)},
		ticks_{}, names_{String8{}}, paths_{fs::path{}}
	{
	}
}
