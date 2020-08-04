#pragma once
#include <unordered_map>
#include "Memory.hpp"

CORE_BEGIN

template <class Key, class T, class Hash = std::hash<Key>, class KeyEqual = std::equal_to<>, class Alloc = PoolAllocator<std::pair<const Key, T>>>
using HashMap = std::unordered_map<Key, T, Hash, KeyEqual, Alloc>;

template <class Key, class T, class Hash = std::hash<Key>, class KeyEqual = std::equal_to<>, class Alloc = PoolAllocator<std::pair<const Key, T>>>
using HashMultiMap = std::unordered_multimap<Key, T, Hash, KeyEqual, Alloc>;

CORE_END
