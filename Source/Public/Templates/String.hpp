#pragma once
#include <string>
#include "Memory.hpp"

#define TEXT(x) L ## x

namespace oeng
{
	template <class CharT, class Traits = std::char_traits<CharT>, class Alloc = RawAllocator<CharT>>
	using BasicString = std::basic_string<CharT, Traits, Alloc>;

	using Char = char;
	using String = BasicString<Char>;
}
