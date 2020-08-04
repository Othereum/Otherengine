#pragma once
#include <vector>
#include "Memory.hpp"

namespace oeng::core
{
	template <class T, class Alloc = PoolAllocator<T>>
	using DyArr = std::vector<T, Alloc>;
}
