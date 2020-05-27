#include <gtest/gtest.h>
#include "Templates/SharedPtr.hpp"

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

	EXPECT_EQ(n, 0);

	{
		SharedPtr<Foo> sh2{new Foo{n}};
		auto sh3 = sh2;
		EXPECT_EQ(sh2.UseCount(), 2);
		EXPECT_EQ(sh3.UseCount(), 2);
	}

	EXPECT_EQ(n, 2);

	{
		SharedPtr<Foo> sh4{new Foo{n}, D{n}};
		SharedPtr<Foo> sh5{new Foo{n}, [&](auto p) { ++n; delete p; }};
	}

	EXPECT_EQ(n, 8);
}
