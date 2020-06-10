#include <gtest/gtest.h>
#include "Templates/Memory.hpp"

namespace oeng
{
	template <class Al>
	static void Benchmark(Al al)
	{
		using T = std::allocator_traits<Al>;
		for (auto i=0; i<1000000; ++i)
			T::deallocate(al, T::allocate(al, 1), 1);
	}

	extern OE_IMPORT bool engine_exist;

	TEST(MemBench, Pool)
	{
		engine_exist = true;
		Benchmark(oeng::PoolAllocator<double>{});
	}

	TEST(MemBench, Raw)
	{
		Benchmark(oeng::RawAllocator<double>{});
	}
}
