#pragma once
#include <vector>
#include "Memory.hpp"

namespace oeng
{
	template <class T, class Alloc = RawAllocator<T>>
	using DyArr = std::vector<T, Alloc>;
}
