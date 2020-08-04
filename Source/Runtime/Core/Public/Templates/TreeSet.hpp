#pragma once
#include <set>
#include "Memory.hpp"

CORE_BEGIN

template <class Key, class Compare = std::less<>, class Alloc = PoolAllocator<Key>>
using TreeSet = std::set<Key, Compare, Alloc>;

template <class Key, class Compare = std::less<>, class Alloc = PoolAllocator<Key>>
using TreeMultiSet = std::multiset<Key, Compare, Alloc>;

CORE_END
