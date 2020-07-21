#include "EngineBase.hpp"
#include "otm/Basic.hpp"

namespace oeng::core
{
	EngineBase* const kEngineBase = nullptr;

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

	RegisterEngineBase::RegisterEngineBase(EngineBase* engine)
	{
		assert(engine);
		assert(kEngineBase);
		const_cast<EngineBase*&>(kEngineBase) = engine;
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

	EngineBase::EngineBase(std::u8string game_module_path)
		:RegisterEngineBase(this),
		CoreSystem{std::move(game_module_path)},
		ticks_{}, names_{String8{}}, paths_{fs::path{}}
	{
	}
}
