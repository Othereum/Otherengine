#include <thread>
#include <vector>
#include <gtest/gtest.h>
#include "Templates/Memory.hpp"

namespace oeng
{
	namespace core
	{
		extern OE_IMPORT bool engine_exist;
	}

	template <class Al>
	static void Benchmark(Al al)
	{
		using T = std::allocator_traits<Al>;
		for (auto i=0; i<1000000; ++i)
			T::deallocate(al, T::allocate(al, 1), 1);
	}

	template <class Al>
	static void BenchmarkMt(Al al)
	{
		using T = std::allocator_traits<Al>;
		auto do_alloc = [&]()
		{
			for (auto i=0; i<1000000; ++i)
				T::deallocate(al, T::allocate(al, 1), 1);
		};

		const auto size = std::thread::hardware_concurrency();
		std::vector<std::thread> threads;
		threads.reserve(size);
		for (auto i=0u; i<size; ++i) threads.emplace_back(do_alloc);
		for (auto& t : threads) t.join();
	}

	TEST(MemBench, Pool)
	{
		engine_exist = true;
		Benchmark(oeng::PoolAllocator<double>{});
	}

	TEST(MemBench, Raw)
	{
		engine_exist = true;
		Benchmark(oeng::RawAllocator<double>{});
	}
	
	TEST(MemBench, PoolMt)
	{
		engine_exist = true;
		BenchmarkMt(oeng::PoolAllocator<double>{});
	}

	TEST(MemBench, RawMt)
	{
		engine_exist = true;
		BenchmarkMt(oeng::RawAllocator<double>{});
	}
}
