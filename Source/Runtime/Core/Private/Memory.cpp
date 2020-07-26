#include "Memory.hpp"
#include <sstream>
#include "Log.hpp"
#include "otm/Basic.hpp"
#include "Templates/DyArr.hpp"

namespace oeng::core
{
	struct MemPoolStorage
	{
		DELETE_CPMV(MemPoolStorage);
		MemPoolStorage() {}
		~MemPoolStorage() {}
		union { omem::MemoryPoolManager pool; };
		bool is_initialized = false;
	};

	static thread_local MemPoolStorage storage;
	
	void InitMemPool()
	{
		assert(!storage.is_initialized);
		new (&storage.pool) omem::MemoryPoolManager;
		storage.is_initialized = true;
	}

	omem::MemoryPoolManager& GetMemPool() noexcept
	{
		assert(storage.is_initialized);
		return storage.pool;
	}

	static String8 ThreadId()
	{
		std::basic_stringstream<char, std::char_traits<char>, PoolAllocator<char>> ss;
		ss << std::this_thread::get_id();
		return AsString8(ss.str());
	}

	void LogMemPoolStatus()
	{
		using namespace otm;
		assert(storage.is_initialized);
		auto& pools = storage.pool.Pools();
		
		omem::PoolInfo max;
		DyArr<omem::PoolInfo> infos;
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
		
		log::Debug(u8"[Mem] Memory pool info for thread {}"sv, ThreadId());
		for (const auto& info : infos)
		{
			log::Debug(u8"[Mem] {:>{}}-byte blocks, total: {:>{}}, peak: {:>{}}, fault: {:>{}}, in use: {}"sv,
				info.size, align.size, info.count, align.count, info.peak, align.peak, info.fault, align.fault, info.cur);
		}
	}

	void CleanUpMemPool() noexcept
	{
		assert(storage.is_initialized);
		
		for (const auto& [size, pool] : storage.pool.Pools())
		{
			if (pool.GetInfo().cur > 0)
			{
				log::Warn(u8"[Mem] Memory leak detected from thread {}"sv, ThreadId());
				break;
			}
		}
		
		storage.pool.~MemoryPoolManager();
		storage.is_initialized = false;
	}
}
