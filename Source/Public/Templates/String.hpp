#pragma once
#include <string>
#include "Memory.hpp"

namespace oeng
{
	template <class CharT, class Traits = std::char_traits<CharT>, class Alloc = RawAllocator<CharT>>
	using BasicString = std::basic_string<CharT, Traits, Alloc>;

	using String = BasicString<char>;
}
