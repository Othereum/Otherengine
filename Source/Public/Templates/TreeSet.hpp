#pragma once
#include <set>
#include "Memory.hpp"

namespace oeng
{
	template <class Key, class Compare = std::less<>, class Alloc = Allocator<Key>>
	using TreeSet = std::set<Key, Compare, Alloc>;
	
	template <class Key, class Compare = std::less<>, class Alloc = Allocator<Key>>
	using TreeMultiSet = std::multiset<Key, Compare, Alloc>;
}
