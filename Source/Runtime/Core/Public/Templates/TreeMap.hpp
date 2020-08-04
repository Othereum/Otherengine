#pragma once
#include <map>
#include "Memory.hpp"

CORE_BEGIN

template <class Key, class T, class Compare = std::less<>, class Alloc = PoolAllocator<std::pair<const Key, T>>>
using TreeMap = std::map<Key, T, Compare, Alloc>;

template <class Key, class T, class Compare = std::less<>, class Alloc = PoolAllocator<std::pair<const Key, T>>>
using TreeMultiMap = std::multimap<Key, T, Compare, Alloc>;

CORE_END
