#include "EngineBase.hpp"
#include <csignal>
#include "otm/Basic.hpp"

namespace oeng::core
{
	static EngineBase* engine_base = nullptr;

	static void LogMemoryInfo(const omem::MemoryPoolManager& pool_manager)
	{
		auto& pools = pool_manager.Pools();
		
		omem::PoolInfo max;
		std::vector<omem::PoolInfo> infos;
		infos.reserve(pools.size());
		
		for (const auto& [size, pool] : pools)
		{
			auto compare = [](const omem::PoolInfo& a, const omem::PoolInfo& b) noexcept
			{
				return a.size < b.size;
			};
			const auto& info = pool.GetInfo();
			const auto pos = std::upper_bound(infos.begin(), infos.end(), info, compare);
			infos.insert(pos, info);

			max.size = Max(max.size, info.size);
			max.count = Max(max.count, info.count);
			max.cur = Max(max.cur, info.cur);
			max.peak = Max(max.peak, info.peak);
			max.fault = Max(max.fault, info.fault);
		}

		omem::PoolInfo align;
		align.size = Log(max.size, 10) + 1;
		align.count = Log(max.count, 10) + 1;
		align.peak = Log(max.peak, 10) + 1;
		align.fault = Log(max.fault, 10) + 1;
		
		for (const auto& info : infos)
		{
			log::Debug(u8"[Mem] {:>{}}-byte blocks, total: {:>{}}, peak: {:>{}}, fault: {:>{}}, leaked: {}"sv,
				info.size, align.size, info.count, align.count, info.peak, align.peak, info.fault, align.fault, info.cur);
		}

		if (max.cur > 0) log::Warn(u8"[Mem] Memory leak detected!"sv);
	}

	static void OnIllegal(int)
	{
		log::Critical(u8"ILLEGAL INSTRUCTION: It's may be because current CPU is not supported."sv);
	}
	
	static void CheckCpu()
	{
		const auto& cpu = CpuInfo::Get();
		log::Info(cpu.GetBrand());
#ifdef OE_USE_AVX2
		if (!cpu.AVX2()) throw std::runtime_error{"Unsupported CPU (AVX2)"};
#endif
	}
	
	RegisterEngineBase::RegisterEngineBase(EngineBase* engine)
	{
		std::signal(SIGILL, &OnIllegal);
		
		if (IsDebugging()) log::Info(u8"Debugger detected"sv);
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
		:thread_id_{std::this_thread::get_id()},
		game_dll_{std::move(game_module_path)},
		game_name_{game_dll_.GetSymbol<const char8_t* const>(u8"kGameName"sv)}
	{
	}

	CoreSystem::~CoreSystem()
	{
		try
		{
			LogMemoryInfo(mem_pool_);
		}
		catch (const std::exception& e)
		{
			log::Error(u8"MemoryPoolWrapper::~MemoryPoolWrapper(): {}", What(e));
		}
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
