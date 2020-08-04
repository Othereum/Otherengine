#pragma once
#include <vector>
#include "Memory.hpp"

CORE_BEGIN

template <class T, class Alloc = PoolAllocator<T>>
using DyArr = std::vector<T, Alloc>;

CORE_END
