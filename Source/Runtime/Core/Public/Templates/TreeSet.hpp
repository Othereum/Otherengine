#pragma once
#include <set>
#include "Memory.hpp"

namespace oeng::core
{
	template <class Key, class Compare = std::less<>, class Alloc = PoolAllocator<Key>>
	using TreeSet = std::set<Key, Compare, Alloc>;
	
	template <class Key, class Compare = std::less<>, class Alloc = PoolAllocator<Key>>
	using TreeMultiSet = std::multiset<Key, Compare, Alloc>;
}
