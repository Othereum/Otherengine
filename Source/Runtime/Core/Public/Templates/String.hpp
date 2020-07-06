#pragma once
#include <string>
#include "Memory.hpp"

namespace oeng
{
	using String = std::basic_string<char8_t, std::char_traits<char8_t>, PoolAllocator<char8_t>>;
	using String16 = std::basic_string<char16_t, std::char_traits<char16_t>, PoolAllocator<char16_t>>;
	
	OEAPI String ToUtf8(std::u16string_view utf16);
	OEAPI String16 ToUtf16(std::u8string_view utf8);
}
