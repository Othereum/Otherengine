#pragma once
#include <string>
#include "Memory.hpp"

namespace oeng
{
	using String = std::basic_string<char8_t, std::char_traits<char8_t>, PoolAllocator<char8_t>>;
}
