#include <gtest/gtest.h>
#include "Memory.hpp"

using namespace oeng;

TEST(MemoryPool, Single)
{
	auto p = Allocate<int>(1);
	Deallocate(p, 1);
}

TEST(MemoryPool, Array)
{
	auto arr = Allocate<int>(10);
	Deallocate(arr, 10);
}

TEST(MemoryPool, Repeat)
{
	for (auto i=0; i<10; ++i)
		Deallocate(Allocate<int>(1), 1);
}

TEST(MemoryPool, RepeatArr)
{
	for (auto i=0; i<10; ++i)
		Deallocate(Allocate<int>(10), 10);
}

TEST(MemoryPool, Arr2)
{
	auto arr = Allocate<int*>(10);
	for (auto i=0; i<10; ++i)
		arr[i] = Allocate<int>(10);
	for (auto i=0; i<10; ++i)
		Deallocate(arr[i], 10);
	Deallocate(arr, 10);
}

constexpr size_t kOne = 1;
constexpr size_t kBytes = 1024*1024;
constexpr size_t kLog2 = otm::Log2Ceil(kBytes);

TEST(MemoryPool, Huge)
{
	Deallocate(Allocate<int>(kBytes), kBytes);
}

TEST(MemoryPool, Increase)
{
	auto arr = Allocate<int*>(kLog2);
	for (size_t i = 0; i < kLog2; ++i)
		arr[i] = Allocate<int>(kOne << i);
	for (size_t i = 0; i < kLog2; ++i)
		Deallocate(arr[i], kOne << i);
	Deallocate(arr, kLog2);
}

TEST(MemoryPool, IncreaseDealloc)
{
	for (size_t i = 0; i < kLog2; ++i)
		Deallocate(Allocate<int>(kOne << i), kOne << i);
}

struct TestStruct
{
	float data[9];
};

TEST(MemoryPool, HugeStruct)
{
	Deallocate(Allocate<TestStruct>(kBytes), kBytes);
}

TEST(MemoryPool, IncreaseStruct)
{
	auto arr = Allocate<TestStruct*>(kLog2);
	for (size_t i = 0; i < kLog2; ++i)
		arr[i] = Allocate<TestStruct>(kOne << i);
	for (size_t i = 0; i < kLog2; ++i)
		Deallocate(arr[i], kOne << i);
	Deallocate(arr, kLog2);
}

TEST(MemoryPool, IncreaseDeallocStruct)
{
	for (size_t i = 0; i < kLog2; ++i)
		Deallocate(Allocate<TestStruct>(kOne << i), kOne << i);
}

