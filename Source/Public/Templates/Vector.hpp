#pragma once
#include <vector>
#include "Memory.hpp"

namespace oeng
{
	template <class T, class Alloc = Allocator<T>>
	using DyArr = std::vector<T, Alloc>;
}
