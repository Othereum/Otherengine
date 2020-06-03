#include <gtest/gtest.h>
#include "Templates/Memory.hpp"

template <class Al>
static void Benchmark(Al al)
{
	using T = std::allocator_traits<Al>;
	for (auto i=0; i<10000000; ++i)
		T::deallocate(al, T::allocate(al, 1), 1);
}

TEST(MemBench, oeng)
{
	Benchmark(oeng::Allocator<double>{});
}

TEST(MemBench, std)
{
	Benchmark(std::allocator<double>{});
}
