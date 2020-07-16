#include "Templates/String.hpp"
#include <locale>

namespace oeng::core
{
	std::u8string ToUtf8(std::u16string_view utf16)
	{
		auto& f = std::use_facet<std::codecvt<char16_t, char8_t, mbstate_t>>({});
		
		std::mbstate_t mb{};
		std::u8string utf8(utf16.size() * f.max_length(), u8'\0');
		
		const char16_t* from_next;
		char8_t* to_next;
		
		f.out(mb, utf16.data(), utf16.data() + utf16.size(), from_next,
			utf8.data(), utf8.data() + utf8.size(), to_next);
		
		utf8.resize(to_next - utf8.data());
		return utf8;
	}

	std::u16string ToUtf16(std::u8string_view utf8)
	{
		auto& f = std::use_facet<std::codecvt<char16_t, char8_t, mbstate_t>>({});
		
		std::mbstate_t mb{};
		std::u16string utf16(utf8.size(), u'\0');
		
		const char8_t* from_next;
		char16_t* to_next;
		
		f.in(mb, utf8.data(), utf8.data() + utf8.size(), from_next,
			utf16.data(), utf16.data() + utf16.size(), to_next);
		
		utf16.resize(to_next - utf16.data());
		return utf16;
	}
}
