#pragma once
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4566)
#endif
#include <fmt/format.h>
#ifdef _MSC_VER
#pragma warning(pop)
#endif
#include "Templates/String.hpp"

CORE_BEGIN

template <class... Args>
[[nodiscard]] String8 Format(std::u8string_view str, const Args&... args)
{
	fmt::basic_memory_buffer<char8_t, fmt::inline_buffer_size, PoolAllocator<char8_t>> buf;
	fmt::vformat_to(buf, str, fmt::make_format_args<fmt::buffer_context<char8_t>>(args...));
	return {buf.data(), buf.size()};
}

template <class T = std::runtime_error, class... Args>
[[noreturn]] void Throw(std::u8string_view str, const Args&... args)
{
	throw T{AsString(Format(str, args...).c_str())};
}

[[nodiscard]] inline const char8_t* What(const std::exception& e) noexcept
{
	return AsString8(e.what());
}

CORE_END
