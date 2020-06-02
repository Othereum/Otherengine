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
		void operator()(Foo* p) const { ++n; delete p; }
		int& n;
	};

	auto n = 0;

	{
		SharedPtr<Foo> sh1;
	}

	ASSERT_EQ(n, 0);

	{
		SharedPtr<Foo> sh2{new Foo{n}};
		auto sh3 = sh2;
		ASSERT_EQ(sh2.use_count(), 2);
		ASSERT_EQ(sh3.use_count(), 2);
	}

	ASSERT_EQ(n, 2);

	{
		SharedPtr<Foo> sh4{new Foo{n}, D{n}};
		SharedPtr<Foo> sh5{new Foo{n}, [&](auto p) { ++n; delete p; }};
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

TEST(SharedPtr, Multithread)
{
	struct Base
	{
		Base(int& n) :n{n} { ++n; }
		~Base() { ++n; }
		int& n;
	};

	struct Derived : Base
	{
		Derived(int& n) :Base{n} {}
	};

	auto n = 0;

	auto thr = [](SharedPtr<Base, true> p)
	{
		auto lp = p;
	};

	SharedPtr<Base, true> p = MakeShared<Derived, true>(n);
	ASSERT_EQ(p.use_count(), 1);

	std::thread t1{thr, p}, t2{thr, p}, t3{thr, p};
	p.reset();
	ASSERT_EQ(p.use_count(), 0);

	t1.join(); t2.join(); t3.join();
	ASSERT_EQ(n, 2);
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

constexpr auto kCopyCnt = 1000000;

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
