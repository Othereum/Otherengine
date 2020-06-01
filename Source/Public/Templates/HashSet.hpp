#pragma once
#include <unordered_set>
#include "Memory.hpp"

namespace oeng
{
	template <class Key, class Hash = std::hash<Key>, class KeyEqual = std::equal_to<>, class Alloc = Allocator<Key>>
	using HashSet = std::unordered_set<Key, Hash, KeyEqual, Alloc>;

	template <class Key, class Hash = std::hash<Key>, class KeyEqual = std::equal_to<>, class Alloc = Allocator<Key>>
	using HashMultiSet = std::unordered_multiset<Key, Hash, KeyEqual, Alloc>;
}
