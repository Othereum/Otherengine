#include <thread>
#include <gtest/gtest.h>
#include "Templates/Memory.hpp"

using namespace oeng;

TEST(SharedPtr, Basic)
{
	struct Foo
	{
		Foo(int& n) :n{n} { ++n; }
		~Foo() { ++n; }
		int& n;
	};

	struct D
	{
		D(int& n) :n{n} {}
		void operator()(Foo* p) const { ++n; Delete(p); }
		int& n;
	};

	auto n = 0;

	{
		SharedPtr<Foo> sh1;
	}

	ASSERT_EQ(n, 0);

	{
		SharedPtr<Foo> sh2{New<Foo>(n)};
		auto sh3 = sh2;
		ASSERT_EQ(sh2.use_count(), 2);
		ASSERT_EQ(sh3.use_count(), 2);
	}

	ASSERT_EQ(n, 2);

	{
		SharedPtr<Foo> sh4{New<Foo>(n), D{n}};
		SharedPtr<Foo> sh5{New<Foo>(n), [&](auto p) { ++n; Delete(p); }};
	}

	ASSERT_EQ(n, 8);

	{
		auto sh6 = MakeShared<Foo>(n);
		auto sh7 = MakeShared<Foo>(n);
		ASSERT_EQ(n, 10);
		sh6 = sh7;
		ASSERT_EQ(n, 11);
		ASSERT_EQ(sh6.use_count(), 2);
		ASSERT_EQ(sh7.use_count(), 2);
	}

	ASSERT_EQ(n, 12);
	
	{
		auto sh8 = MakeShared<Foo>(n);
		auto sh9 = MakeShared<Foo>(n);
		ASSERT_EQ(n, 14);
		sh8 = std::move(sh9);
		ASSERT_EQ(n, 15);
		ASSERT_EQ(sh8.use_count(), 1);
		ASSERT_EQ(sh9.use_count(), 0);
		ASSERT_EQ(sh9, nullptr);
	}

	ASSERT_EQ(n, 16);
}

TEST(SharedPtr, WeakPtr)
{
	WeakPtr<int> weak;
	ASSERT_TRUE(weak.expired());
	{
		auto sp = MakeShared<int>(42);
		weak = sp;
		ASSERT_EQ(weak.use_count(), 1);
		auto spt = weak.lock();
		ASSERT_EQ(weak.use_count(), 2);
		ASSERT_TRUE(spt);
		ASSERT_EQ(*spt, 42);
	}
	ASSERT_EQ(weak.use_count(), 0);
	ASSERT_TRUE(weak.expired());
}

constexpr auto kCopyCnt = 10000000;
constexpr auto kCopyCntMt = 1000000;

TEST(SharedPtr, Bench_NonThreadSafe)
{
	auto ptr = MakeShared<int, false>();
	for (auto i=0; i<kCopyCnt; ++i) { auto copied = ptr; }
}

TEST(SharedPtr, Bench_ThreadSafe)
{
	auto ptr = MakeShared<int, true>();
	for (auto i=0; i<kCopyCnt; ++i) { auto copied = ptr; }
}

TEST(SharedPtr, Bench_Standard)
{
	auto ptr = std::make_shared<int>();
	for (auto i=0; i<kCopyCnt; ++i) { auto copied = ptr; }
}

template <class Weak, class Shared>
void MtBench(Shared ptr)
{
	auto thr = [](Shared ptr)
	{
		for (auto i=0; i<kCopyCntMt; ++i) { auto copied = ptr; }
	};

	auto thr2 = [](Weak weak)
	{
		for (auto i=0; i<kCopyCntMt; ++i)
		{
			auto copied = weak;
			weak.lock();
		}
	};

	std::thread t1{thr, ptr}, t2{thr, ptr};
	Weak weak = ptr;
	ptr.reset();
	std::thread t3{thr2, weak};
	t1.join(); t2.join(); t3.join();
	ASSERT_TRUE(weak.expired());
}

TEST(SharedPtr, Multithread)
{
	MtBench<WeakPtr<int, true>>(MakeShared<int, true>());
}

TEST(SharedPtr, MultithreadStd)
{
	MtBench<std::weak_ptr<int>>(std::make_shared<int>());
}
